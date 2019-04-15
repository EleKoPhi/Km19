﻿// HttpWindowsTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WebServer.h"

int main()
{
	WebServer server;
	server.init();
	while(true)
	{
		try
		{
			server.run();
		}
		catch(exception exc)
		{
			printf(exc.what());
			return 1;
		}
	}
	return 0;
}

