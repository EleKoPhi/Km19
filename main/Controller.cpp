#include "Helper.h"
#include "UserHandler.h"
#include "Drawer.h"
#include "Controller.h"

Controller::Controller(int chipSelect, int slaveSelect, int rstPin, int clk, int data) : _drawer(clk, data) {}

void Controller::Begin()
{
	auto users = UserHandler::getInstance();
	_drawer.DrawErr(users->SdStatus, users->NfcStatus, users->RtcStatus);
	_drawer.DrawMain();
	this->Reset();
}

void Controller::setCurrentStatus(MillStates stat)
{
	_currentStatus = stat;
}

MillStates Controller::getCurrentStatus()
{
	return _currentStatus;
}

MillStates Controller::updateStateTransitions()
{
	auto users = UserHandler::getInstance();
	MillStates newStatus = users->readButtonChoice();
	if((_currentStatus == WaitForCard || _currentStatus == LastUserState || _currentStatus == KaffeeKingState || _currentStatus == CurrentDrawsState) && users->isCardAvailable())
	{
		return beginState(ReadCard);
	}

	else if((_currentStatus == WaitForCard && newStatus == MillStates::Einfach) || (_currentStatus == CurrentDrawsState && newStatus == MillStates::Doppelt))
	{
		return beginState(LastUserState);
	}

	else if((_currentStatus == LastUserState && newStatus == MillStates::Einfach) || (_currentStatus == KaffeeKingState && newStatus == MillStates::Doppelt))
	{
		return beginState(CurrentDrawsState);
	}

	else if((_currentStatus == CurrentDrawsState && newStatus == MillStates::Einfach) || (_currentStatus == WaitForCard && newStatus == MillStates::Doppelt))
	{
		return beginState(KaffeeKingState);
	}

	else if((_currentStatus == KaffeeKingState && newStatus == MillStates::Einfach) || (_currentStatus == LastUserState && newStatus == MillStates::Doppelt))
	{
		return beginState(WaitForCard);
	}

	else if(_currentStatus == ReadCard && _currentUser == UserHandler::UnknownUser)
	{
		return beginState(UnknownUserState);
	}

	else if(_currentStatus == ReadCard && _currentUser != UserHandler::UnknownUser && _currentUser != "")
	{
		return beginState(WaitForInput);
	}

	else if(_currentStatus == WaitForInput && (newStatus == MillStates::Einfach))
	{
		users->writeLog(_currentUserId, LogEntryType::Single);
		return beginState(Einfach);
	}

	else if(_currentStatus == WaitForSplitBooking && (newStatus == MillStates::Einfach || newStatus == MillStates::Doppelt))
	{
		users->writeLog(_currentUserId, LogEntryType::Double);
		return beginState(Doppelt);
	}

	else if(_currentStatus == WaitForInput && newStatus == MillStates::Doppelt)
	{
		return beginState(WaitForSplitBooking);
	}

	else if(_currentStatus == WaitForSplitBooking && users->isCardAvailable())
	{
		return beginState(IdentSecondPayer);
	}

	else
	{
		return _currentStatus;
	}

}

bool Controller::TimeOut(int time)
{
	if(_deltaTime >= time)
	{
		this->Reset();
		return true;
	}
	return false;
}

unsigned long Controller::updateDeltaTime()
{
	_deltaTime = millis() - _startTime;
}

void Controller::States(MillStates Status)
{
	auto users = UserHandler::getInstance();
	if(Status == WaitForCard)
	{
		_drawer.DrawMain();
	}

	if(Status == ReadCard)
	{
		if(users->isCardAvailable())
		{
			_currentUserId = users->readCurrentCardId();
			_currentUser = users->checkUser(_currentUserId);
		}
	}

	if(Status == WaitForInput)
	{
		this->_drawer.DrawDes(_currentUser);
		this->TimeOut(10000);
	}

	if(Status == Einfach)
	{
		this->_drawer.DisplayProgress(((_deltaTime * 100) / 25) / 100);
		this->TimeOut(2500);
	}

	if(Status == Doppelt)
	{
		this->_drawer.DisplayProgress(((_deltaTime * 10) / 5) / 100);
		this->TimeOut(5000);
	}

	if(Status == WaitForSplitBooking)
	{
		this->_drawer.DrawSplitQ();
		this->TimeOut(10000);
	}

	if(Status == IdentSecondPayer)
	{
		this->TimeOut(10000);

		if(users->isCardAvailable())
		{
			_additionalUserId = users->readCurrentCardId();
			_additionalUser = users->checkUser(_currentUserId);

			if(_currentUser == "Unknow user")
			{
				this->setCurrentStatus(WaitForSplitBooking);
				_additionalUserId = "";
				_additionalUser = "";
			}
			this->setCurrentStatus(Doppelt);
		}
	}

	if(Status == UnknownUserState)
	{
		_drawer.DrawUnknown();
		this->TimeOut(2500);
	}

	if(Status == LastUserState)
	{
		_drawer.DrawLastUser("Test");
		this->TimeOut(10000);
	}

	if(Status == CurrentDrawsState)
	{
		_drawer.DrawCurrentAmount(100);
		this->TimeOut(10000);
	}

	if(Status == KaffeeKingState)
	{
		_drawer.DrawKaffeeKing("Test");
		this->TimeOut(10000);
	}
}

void Controller::Reset()
{
	setCurrentStatus(WaitForCard);
	_deltaTime = 0;
	_currentUser = ("");
	_additionalUser = ("");
	_currentUserId = ("");
	_additionalUserId = ("");
}


string Controller::getCurrentUser()
{
	return _currentUser;
}

void Controller::setCurrentUser(string user)
{
	_currentUser = user;
}

MillStates Controller::beginState(MillStates state)
{
	this->setCurrentStatus(state);
	_startTime = millis();
	this->updateDeltaTime();

	return state;
}
