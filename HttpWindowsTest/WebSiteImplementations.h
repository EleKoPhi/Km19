#pragma once

#include"WebSite.h"

class LogSite : public WebSite
{
public:
	LogSite() : WebSite("log.htm") {}
	~LogSite() {}
	virtual void fillPlaceholders();
	virtual void handleRequest(WebRequest* request);
};

class UserSite : public WebSite
{
public:
	UserSite() : WebSite("users.htm") {}
	~UserSite() {}
	virtual void fillPlaceholders();
	virtual void handleRequest(WebRequest* request);
};

class ConfigSite : public WebSite
{
public:
	ConfigSite() : WebSite("config.htm") {}
	~ConfigSite() {}
	virtual void fillPlaceholders();
	virtual void handleRequest(WebRequest* request);
};

class StartSite : public WebSite
{
public:
	StartSite() : WebSite("start.htm") {}
	~StartSite() {}
	virtual void fillPlaceholders();
};
