#include "Arduino.h"
#include "UserHandler.h"
#include "Drawer.h"
#include "Controller.h"
#include "defines.h"

Controller :: Controller(int chipSelect, int slaveSelect, int rstPin, int clk, int data) : _drawer(clk, data), _userHandler(chipSelect, slaveSelect, rstPin) {}

void Controller::Begin()
{
  _userHandler.StartUp();
  _drawer.DrawErr(_userHandler.SdStatus, _userHandler.NfcStatus, _userHandler.RtcStatus);  
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
  if ((_currentStatus == WaitForCard || _currentStatus == LastUserState || _currentStatus == KaffeeKingState || _currentStatus== CurrentDrawsState) && _userHandler.HasCardToRead())
  {
    this->PutCurrentStatus(ReadCard);
    return ReadCard;
  }

  if(_currentStatus == WaitForCard && _userHandler.ReadUserInput() == 'l')
  {
	  this->PutCurrentStatus(LastUserState);
	  _startTime = millis();
	  this->UpDateTime();
	  return LastUserState;
  }

  if(_currentStatus == LastUserState && _userHandler.ReadUserInput() == 'l')
  {
	  this->PutCurrentStatus(CurrentDrawsState);
	  _startTime = millis();
	  this->UpDateTime();
	  return CurrentDrawsState;
  }

  if(_currentStatus == CurrentDrawsState && _userHandler.ReadUserInput() == 'l')
  {
	  this->PutCurrentStatus(KaffeeKingState);
	  _startTime = millis();
	  this->UpDateTime();
	  return KaffeeKingState;
  }

  if(_currentStatus == KaffeeKingState && _userHandler.ReadUserInput() == 'l')
  {
	  this->PutCurrentStatus(WaitForCard);
	  _startTime = millis();
	  this->UpDateTime();
	  return WaitForCard;
  }

  if(_currentStatus == WaitForCard && _userHandler.ReadUserInput() == 'r')
  {
	  Serial.println("Go to KaffeKingState");
	  this->PutCurrentStatus(KaffeeKingState);
	  _startTime = millis();
	  this->UpDateTime();
	  return KaffeeKingState;
  }

  if(_currentStatus == KaffeeKingState && _userHandler.ReadUserInput() == 'r')
  {
	  this->PutCurrentStatus(CurrentDrawsState);
	  _startTime = millis();
	  this->UpDateTime();
	  return CurrentDrawsState;
  }

  if(_currentStatus == CurrentDrawsState && _userHandler.ReadUserInput() == 'r')
  {
	  this->PutCurrentStatus(LastUserState);
	  _startTime = millis();
	  this->UpDateTime();
	  return LastUserState;
  }

  if(_currentStatus == LastUserState && _userHandler.ReadUserInput() == 'r')
  {
	  this->PutCurrentStatus(WaitForCard);
	  _startTime = millis();
	  this->UpDateTime();
	  return WaitForCard;
  }

  if (_currentStatus == ReadCard && _currentUser == "Unknow user")
  {
    this->PutCurrentStatus(UnknownUserState);
    _startTime = millis();
    this->UpDateTime();
    return UnknownUserState;
  }

  if (_currentStatus == ReadCard && _currentUser != "Unknown user" && _currentUser != String(""))
  {
    this->PutCurrentStatus(WaitForInput);
    _startTime = millis();
    this->UpDateTime();
    return WaitForInput;
  }

  if (_currentStatus == WaitForInput && (_userHandler.ReadUserInput() == 'l'))
  {
    this->PutCurrentStatus(Einfach);
    _startTime = millis();
    this->UpDateTime();
	_userHandler.WriteToLog(Einfach, _currentUser.c_str(), _additionalUser.c_str(), _currentUserId.c_str(), _additionalUserId.c_str());
    return Einfach;
  }

  if (_currentStatus == WaitForSplitBooking && (_userHandler.ReadUserInput() == 'l' || _userHandler.ReadUserInput() == 'r'))
  {
    this->PutCurrentStatus(Doppelt);
    _startTime = millis();
    this->UpDateTime();
	_userHandler.WriteToLog(Doppelt, _currentUser.c_str(), _additionalUser.c_str(),_currentUserId.c_str(), _additionalUserId.c_str());
    return Doppelt;
  }

  if (_currentStatus == WaitForInput && _userHandler.ReadUserInput() == 'r') 
  {
    this->PutCurrentStatus(WaitForSplitBooking);
    _startTime = millis();
    this->UpDateTime();
    return WaitForSplitBooking;
  }

  if (_currentStatus == WaitForSplitBooking && _userHandler.HasCardToRead())
  {
    this->PutCurrentStatus(IdentSecondPayer);
    _startTime = millis();
    this->UpDateTime();
    return IdentSecondPayer;
  }

  return _currentStatus;
}

bool Controller::TimeOut(int time)
{
  if (_deltaTime >= time)
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

  if (Status == WaitForCard)
  {
    _drawer.DrawMain();
  }

  if (Status == ReadCard)
  {
    if (_userHandler.HasCardToRead())
    {
      _currentUserId = _userHandler.GetCardId();
      _currentUser = _userHandler.CheckIfExists(_currentUserId.c_str());
    }
  }

  if (Status == WaitForInput)
  {
    this->_drawer.DrawDes(String(_currentUser));
    this->TimeOut(10000);
  }

  if (Status == Einfach)
  {
    int progress = ((_deltaTime*100)/25)/100;
    this->_drawer.DisplayProgress(progress);
	this->TimeOut(2500);
  }

  if (Status == Doppelt)
  {
    int progress = ((_deltaTime*10)/5)/100;
    this->_drawer.DisplayProgress(progress);
	this->TimeOut(5000);
  }

  if (Status == WaitForSplitBooking)
  {
    this->_drawer.DrawSplitQ();
    this->TimeOut(10000);
  }

  if (Status == IdentSecondPayer)
  {
    this->TimeOut(10000);
    
    if (_userHandler.HasCardToRead())
    {
      _additionalUserId = _userHandler.GetCardId();
      _additionalUser = _userHandler.CheckIfExists(_currentUserId.c_str());

      if (_currentUser == String("Unknow user"))
      {
        this->PutCurrentStatus(WaitForSplitBooking);
        _additionalUserId = "";
        _additionalUser = "";
      }
      this->PutCurrentStatus(Doppelt);
    }
  }

  if (Status == UnknownUserState)
  {
    _drawer.DrawUnknown();
    this->TimeOut(2500);
  }

  if(Status == LastUserState)
  {
	  _drawer.DrawLastUser("Test");
	  //_userHandler.GetLastUser();
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
