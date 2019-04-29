#include "Helper.h"
#include "UserHandler.h"
#include "Drawer.h"
#include "Controller.h"
#include "defines.h"

Controller::Controller(int chipSelect, int slaveSelect, int rstPin, int clk, int data) : _drawer(clk, data) {}

void Controller::Begin()
{
	auto _userHandler = UserHandler::getInstance();
	_drawer.DrawErr(_userHandler->SdStatus, _userHandler.NfcStatus, _userHandler.RtcStatus);
	_drawer.DrawMain();
	this->Reset();
}

void Controller::PutCurrentStatus(char stat)
{
	_currentStatus = stat;
}

char Controller::GetCurrentStatus()
{
	return _currentStatus;
}

char Controller::StateTransitions()
{
	char newStatus = _userHandler.ReadUserInput();
	if((_currentStatus == WaitForCard || _currentStatus == LastUserState || _currentStatus == KaffeeKingState || _currentStatus == CurrentDrawsState) && _userHandler.HasCardToRead())
	{
		return StateBegin(ReadCard);
	}

	else if((_currentStatus == WaitForCard && newStatus == 'l') || (_currentStatus == CurrentDrawsState && newStatus == 'r'))
	{
		return StateBegin(LastUserState);
	}

	else if((_currentStatus == LastUserState && newStatus == 'l') || (_currentStatus == KaffeeKingState && newStatus == 'r'))
	{
		return StateBegin(CurrentDrawsState);
	}

	else if((_currentStatus == CurrentDrawsState && newStatus == 'l') || (_currentStatus == WaitForCard && newStatus == 'r'))
	{
		return StateBegin(KaffeeKingState);
	}

	else if((_currentStatus == KaffeeKingState && newStatus == 'l') || (_currentStatus == LastUserState && newStatus == 'r'))
	{
		return StateBegin(WaitForCard);
	}

	else if(_currentStatus == ReadCard && _currentUser == UserHandler::UnknownUser)
	{
		return StateBegin(UnknownUserState);
	}

	else if(_currentStatus == ReadCard && _currentUser != UserHandler::UnknownUser && _currentUser != "")
	{
		return StateBegin(WaitForInput);
	}

	else if(_currentStatus == WaitForInput && (newStatus == 'l'))
	{
		_userHandler.WriteToLog(Einfach, _currentUser.c_str(), _additionalUser.c_str(), _currentUserId.c_str(), _additionalUserId.c_str());
		return StateBegin(Einfach);
	}

	else if(_currentStatus == WaitForSplitBooking && (newStatus == 'l' || newStatus == 'r'))
	{
		_userHandler.WriteToLog(Doppelt, _currentUser.c_str(), _additionalUser.c_str(), _currentUserId.c_str(), _additionalUserId.c_str());
		return StateBegin(Doppelt);
	}

	else if(_currentStatus == WaitForInput && newStatus == 'r')
	{
		return StateBegin(WaitForSplitBooking);
	}

	else if(_currentStatus == WaitForSplitBooking && _userHandler.HasCardToRead())
	{
		return StateBegin(IdentSecondPayer);
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

void Controller::UpDateTime()
{
	_deltaTime = millis() - _startTime;
}

void Controller::States(char Status)
{

	if(Status == WaitForCard)
	{
		_drawer.DrawMain();
	}

	if(Status == ReadCard)
	{
		if(_userHandler.HasCardToRead())
		{
			_currentUserId = _userHandler.GetCardId();
			_currentUser = _userHandler.checkUser(_currentUserId.c_str());
		}
	}

	if(Status == WaitForInput)
	{
		this->_drawer.DrawDes(String(_currentUser));
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

		if(_userHandler.HasCardToRead())
		{
			_additionalUserId = _userHandler.GetCardId();
			_additionalUser = _userHandler.checkUser(_currentUserId.c_str());

			if(_currentUser == String("Unknow user"))
			{
				this->PutCurrentStatus(WaitForSplitBooking);
				_additionalUserId = "";
				_additionalUser = "";
			}
			this->PutCurrentStatus(Doppelt);
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
	this->PutCurrentStatus(WaitForCard);
	_deltaTime = 0;
	_currentUser = String("");
	_additionalUser = String("");
	_currentUserId = String("");
	_additionalUserId = String("");
}


String Controller::GetCurrentUser()
{
	return this->_currentUser;
}

void Controller::SetCurrentUser(String user)
{
	_currentUser = user;
}

char Controller::StateBegin(char state)
{
	this->PutCurrentStatus(state);
	_startTime = millis();
	this->UpDateTime();

	return state;
}
