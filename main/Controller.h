#ifndef CONTROLLER_h
#define CONTROLLER_h

#include "Helper.h"
#include "UserHandler.h"
#include "Drawer.h"
#include "WebServer.h"

class Controller
{
public:
	Controller();
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
	Drawer drawer;
	WebServer webServer;
	MillStates currentStatus;

	string currentUser;
	string currentUserId;

	string additionalUser;
	string additionalUserId;

	unsigned long startTime, currentTime, deltaTime;
};


#endif
