#ifndef CONTROLLER_h
#define CONTROLLER_h

#include "Helper.h"
#include "UserHandler.h"
#include "Drawer.h"
#include "WebServer.h"

class Controller
{
public:
	Controller(int chipSelect, int slaveSelect, int rstPin, int clk, int data);
	void Begin();
	void setCurrentStatus(MillStates stat);
	MillStates updateStateTransitions();
	MillStates getCurrentStatus();
	unsigned long updateDeltaTime();
	void States(MillStates Status);
	bool TimeOut(int time);
	void Reset();
	MillStates beginState(MillStates state);

	string getCurrentUser();
	void setCurrentUser(string user);

private:
	Drawer _drawer;
	WebServer _webserver;
	MillStates _currentStatus;

	string _currentUser;
	string _currentUserId;

	string _additionalUser;
	string _additionalUserId;

	unsigned long _startTime, _currentTime, _deltaTime;
};


#endif
