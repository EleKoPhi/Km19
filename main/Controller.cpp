#include "FileIO.h"
#include "Helper.h"
#include "UserHandler.h"
#include "Drawer.h"
#include "Controller.h"

Controller::Controller() : drawer() {}

void Controller::Begin()
{
	auto users = UserHandler::getInstance();
	drawer.DrawErr(users->SdStatus, users->NfcStatus, users->RtcStatus);
	drawer.DrawMain();
	webServer.init();
	this->Reset();
}

void Controller::setCurrentStatus(MillStates stat)
{
	currentStatus = stat;
}

MillStates Controller::getCurrentStatus()
{
	return currentStatus;
}

MillStates Controller::updateStateTransitions()
{
	webServer.run();
	auto users = UserHandler::getInstance();
	MillStates newStatus = users->readButtonChoice();
	if((currentStatus == WaitForCard || currentStatus == LastUserState || currentStatus == KaffeeKingState || currentStatus == CurrentDrawsState) && users->isCardAvailable())
	{
		return beginState(ReadCard);
	}

	else if((currentStatus == WaitForCard && newStatus == MillStates::Einfach) || (currentStatus == CurrentDrawsState && newStatus == MillStates::Doppelt))
	{
		return beginState(LastUserState);
	}

	else if((currentStatus == LastUserState && newStatus == MillStates::Einfach) || (currentStatus == KaffeeKingState && newStatus == MillStates::Doppelt))
	{
		return beginState(CurrentDrawsState);
	}

	else if((currentStatus == CurrentDrawsState && newStatus == MillStates::Einfach) || (currentStatus == WaitForCard && newStatus == MillStates::Doppelt))
	{
		return beginState(KaffeeKingState);
	}

	else if((currentStatus == KaffeeKingState && newStatus == MillStates::Einfach) || (currentStatus == LastUserState && newStatus == MillStates::Doppelt))
	{
		return beginState(WaitForCard);
	}

	else if(currentStatus == ReadCard && currentUser == UserHandler::UnknownUser)
	{
		return beginState(UnknownUserState);
	}

	else if(currentStatus == ReadCard && currentUser != UserHandler::UnknownUser && currentUser != "")
	{
		return beginState(WaitForInput);
	}

	else if(currentStatus == WaitForInput && (newStatus == MillStates::Einfach))
	{
		users->writeLog(currentUserId, LogEntryType::Single);
		return beginState(Einfach);
	}

	else if(currentStatus == WaitForSplitBooking && (newStatus == MillStates::Einfach || newStatus == MillStates::Doppelt))
	{
		users->writeLog(currentUserId, LogEntryType::Double);
		return beginState(Doppelt);
	}

	else if(currentStatus == WaitForInput && newStatus == MillStates::Doppelt)
	{
		return beginState(WaitForSplitBooking);
	}

	else if(currentStatus == WaitForSplitBooking && users->isCardAvailable())
	{
		return beginState(IdentSecondPayer);
	}

	else
	{
		return currentStatus;
	}

}

bool Controller::TimeOut(int time)
{
	if(deltaTime >= time)
	{
		this->Reset();
		return true;
	}
	return false;
}

unsigned long Controller::updateDeltaTime()
{
	deltaTime = millis() - startTime;
}

void Controller::States(MillStates Status)
{
	auto users = UserHandler::getInstance();
	if(Status == WaitForCard)
	{
		drawer.DrawMain();
	}

	if(Status == ReadCard)
	{
		if(users->isCardAvailable())
		{
			currentUserId = users->readCurrentCardId();
			currentUser = users->checkUser(currentUserId);
		}
	}

	if(Status == WaitForInput)
	{
		this->drawer.DrawDes(currentUser);
		this->TimeOut(10000);
	}

	if(Status == Einfach)
	{
		this->drawer.DisplayProgress(((deltaTime * 100) / 25) / 100);
		this->TimeOut(2500);
	}

	if(Status == Doppelt)
	{
		this->drawer.DisplayProgress(((deltaTime * 10) / 5) / 100);
		this->TimeOut(5000);
	}

	if(Status == WaitForSplitBooking)
	{
		this->drawer.DrawSplitQ();
		this->TimeOut(10000);
	}

	if(Status == IdentSecondPayer)
	{
		this->TimeOut(10000);

		if(users->isCardAvailable())
		{
			additionalUserId = users->readCurrentCardId();
			additionalUser = users->checkUser(currentUserId);

			if(currentUser == "Unknow user")
			{
				this->setCurrentStatus(WaitForSplitBooking);
				additionalUserId = "";
				additionalUser = "";
			}
			this->setCurrentStatus(Doppelt);
		}
	}

	if(Status == UnknownUserState)
	{
		drawer.DrawUnknown();
		this->TimeOut(2500);
	}

	if(Status == LastUserState)
	{
		drawer.DrawLastUser("Test");
		this->TimeOut(10000);
	}

	if(Status == CurrentDrawsState)
	{
		drawer.DrawCurrentAmount(100);
		this->TimeOut(10000);
	}

	if(Status == KaffeeKingState)
	{
		drawer.DrawKaffeeKing("Test");
		this->TimeOut(10000);
	}
}

void Controller::Reset()
{
	setCurrentStatus(WaitForCard);
	deltaTime = 0;
	currentUser = ("");
	additionalUser = ("");
	currentUserId = ("");
	additionalUserId = ("");
}


string Controller::getCurrentUser()
{
	return currentUser;
}

void Controller::setCurrentUser(string user)
{
	currentUser = user;
}

MillStates Controller::beginState(MillStates state)
{
	this->setCurrentStatus(state);
	startTime = millis();
	this->updateDeltaTime();

	return state;
}
