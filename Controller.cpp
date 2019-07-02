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
  pinMode(5, OUTPUT);
  this->MillOff();
  String part1 = "";
  String part2 = "";

  this->T_einfach = 0;
  this->T_doppelt = 0;

  String configString = _userHandler.GetConfig();

  part1 = configString.substring(0, 5);
  part2 = configString.substring(6, 11);

  this->T_einfach = part1.toInt();
  this->T_doppelt = part2.toInt();

  Serial.println(T_einfach);
  Serial.println(T_doppelt);

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
  if ((_currentStatus == WaitForUser | _currentStatus == ShowCredit) && _userHandler.ReadUserInput() == 'l')
  {
    return StateBegin(PayOne);
  }

  else if ((_currentStatus == WaitForUser  | _currentStatus == ShowCredit) && _userHandler.ReadUserInput() == 'r')
  {
    return StateBegin(SplitPaymentQ);
  }

  else if (_currentStatus == SplitPaymentQ && _userHandler.ReadUserInput() == 'l')
  {
    return StateBegin(PayTwo);
  }

  else if (_currentStatus == SplitPaymentQ && _userHandler.ReadUserInput() == 'r')
  {
    return StateBegin(PayTwo_1);
  }

  else if (_currentStatus == WaitForUser && _userHandler.HasCardToRead())
  {
    return StateBegin(ReadCreditUser);
  }

  else if (_currentStatus == ReadCreditUser)
  {
    return StateBegin(ShowCredit);
  }

  else if (_currentStatus == PayOne && _userHandler.HasCardToRead())
  {
    int Credit = _userHandler.ReadCredit();
    int stat = 0;
    bool err = false;
    int count = 0;

    if (Credit == -1)
    {
      return StateBegin(WaitForUser);
    }

    if (Credit >= 1)
    {
      stat = _userHandler.WriteCredit(Credit - 1,false);
      if (stat == 0)
      {
        delay(100);
        return StateBegin(Einfach);
      }
      else
      {
        while (stat != 0 || count > 10)
        {
          stat = _userHandler.WriteCredit(Credit - 1,false);
          if (stat == 0)break;
          count++;
        }

        if (count > 8)
        {
          _drawer.Err();
          delay(2000);
          return StateBegin(WaitForUser);
        }
        else
        { delay(100);
          return StateBegin(Einfach);
        }
      }
    }
    else
    {
      return StateBegin(LowCredit);
    }
  }

  else if (_currentStatus == PayTwo && _userHandler.HasCardToRead())
  {
    int Credit = _userHandler.ReadCredit();
    int stat = 0;
    bool err = false;
    int count = 0;

    if (Credit == -1)
    {
      return StateBegin(WaitForUser);
    }

    if (Credit >= 2)
    {
      stat = _userHandler.WriteCredit(Credit - 2,true);
      if (stat == 0)
      {
        delay(100);
        return StateBegin(Doppelt);
      }
      else
      {
        while (stat != 0 || count > 10)
        {
          stat = _userHandler.WriteCredit(Credit - 2,true);
          if (stat == 0)break;
          count++;
        }

        if (count > 8)
        {
          _drawer.Err();
          delay(2000);
          return StateBegin(WaitForUser);
        }
        else
        {
          delay(100);
          return StateBegin(Doppelt);
        }
      }
    }
    else
    {
      return StateBegin(LowCredit);
    }
  }

  else if (_currentStatus == PayTwo_1 && _userHandler.HasCardToRead())
  {
    int Credit = _userHandler.ReadCredit();

    if (Credit == -1)
    {
      return StateBegin(WaitForUser);
    }

    long code = 0;
    if (Credit >= 1)
    {
      _userHandler.WriteCredit(Credit - 1,false);
      _userHandler.newRead();
      while (_currentUser == "0" || _currentUser == "") 
      {
        _currentUser = _userHandler.GetCardId();
        _userHandler.ReadCredit();
      }
      return StateBegin(PayTwo_2);
    }
    else
    {
      return StateBegin(LowCredit);
    }

  }

  else if (_currentStatus == PayTwo_2 && _userHandler.HasCardToRead())
  {
    String CurrentUser = _userHandler.GetCardId();
    int Credit = _userHandler.ReadCredit();

    if (Credit == -1)
    {
      return StateBegin(WaitForUser);
    }

    if (Credit >= 1 && _currentUser != CurrentUser && CurrentUser != 0)
    {
      _userHandler.WriteCredit(Credit - 1,false);
      delay(100);
      return StateBegin(Doppelt);
    }
    else
    {
      return StateBegin(PayTwo_2);
    }
  }
  else
  {
    return _currentStatus;
  }
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

  if (Status == Einfach)
  {
    this->MillOn();

    int progress = ((_deltaTime) / (this->T_einfach / 100));
    this->_drawer.DisplayProgress(progress);
    Serial.println(progress);
    this->TimeOut(this->T_einfach);
  }

  else if (Status == Doppelt)
  {
    
    this->MillOn();
    this->_drawer.DisplayProgress(_deltaTime / (this->T_doppelt / 100));
    this->TimeOut(this->T_doppelt);
  }

  else if (Status == WaitForUser)
  {
    _drawer.DrawWaitForUser();
  }

  else if (Status == PayOne)
  {
    _drawer.DrawPayOne();
    this->TimeOut(10000);
  }

  else if (Status == SplitPaymentQ)
  {
    _drawer.DrawSplitQ2();
    this->TimeOut(10000);
  }

  else if (Status == PayTwo)
  {
    _drawer.DrawPay2();
    this->TimeOut(10000);
  }

  else if (Status == PayTwo_1)
  {
    _drawer.DrawPay2_1();
    this->TimeOut(10000);
  }

  if (Status == PayTwo_2)
  {
    _drawer.DrawPay2_2();
    this->TimeOut(30000);
  }

  else if (Status == LowCredit)
  {
    _drawer.DrawLowCredit();
    this->TimeOut(5000);
  }

  else if (Status == ReadCreditUser)
  {
    user = _userHandler.GetCardId().toInt();
    credit = _userHandler.ReadCredit();
  }

  else if (Status == ShowCredit)
  {
    if(user==0)
    {
      this->TimeOut(0);
      return;
    }
    _drawer.DrawCredit(user, credit);
    this->TimeOut(5000);
  }
}

void Controller::Reset()
{
  this->MillOff();
  this->PutCurrentStatus(WaitForUser);
  _deltaTime = 0;
  _currentUser = String("");
  _additionalUser = String("");
  _currentUserId = String("");
  _additionalUserId = String("");
  user = 0;
  credit = 0;
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

void Controller::MillOn()
{
  digitalWrite(RelayPin, HIGH);
}

void Controller::MillOff()
{
  digitalWrite(RelayPin, LOW);
}
