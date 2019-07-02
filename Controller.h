#ifndef CONTROLLER_h
#define CONTROLLER_h

#include "Arduino.h"
#include "UserHandler.h"
#include "Drawer.h"


class Controller
{
  public:
  Controller(int chipSelect, int slaveSelect, int rstPin,int clk, int data);
  void Begin();
  void PutCurrentStatus(char stat);
  char StateTransitions();
  char GetCurrentStatus();
  void UpDateTime();
  void States(char Status);
  bool TimeOut(int time);
  void Reset();
  char StateBegin(char state);
  void MillOn();
  void MillOff();

  String GetCurrentUser();
  void SetCurrentUser(String user);


  int T_einfach;
  int T_doppelt;
  
  
  private:
  Drawer _drawer;
  UserHandler _userHandler;
  //WebServer _wifi;
  char _currentStatus;
  
  String _currentUser;
  String _currentUserId;
  
  String _additionalUser;
  String _additionalUserId;

  int user;
  int credit;
  
  unsigned long _startTime, _currentTime, _deltaTime;
};


#endif
