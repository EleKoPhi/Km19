#include "stdafx.h"
#include "WebSiteImplementations.h"
#include "UserHandler.h"
#include "WebServer.h"

void LogSite::fillPlaceholders()
{
	auto uh = UserHandler::getInstance();
	{
		map<string, string> values{{"currententries", to_string(uh->numberOfLogEntries())}};
		setValues(values);
	}
	{
		map<string, User> users = uh->readUsers();
		map<string, string> values;
		for(auto entry : uh->readLog())
		{
			string name;
			auto namelookup = users.find(entry.cardId);
			if(namelookup != users.end())
				name = namelookup->second.name;
			values["timestamp"] = entry.timestamp.getDateTime();
			values["cardid"] = entry.cardId;
			values["username"] = name;
			values["type"] = to_string(entry.type);
			setGroup("entry", values);
		}
	}
	{
		map<string, string> file;
		for(auto filename : uh->getOldLogFiles())
		{
			file["filename"] = filename;
			setGroup("file", file);
		}
	}
}

void LogSite::handleRequest(WebRequest * request)
{
	WebSite::handleRequest(request);
}

void UserSite::fillPlaceholders()
{
	auto uh = UserHandler::getInstance();
	for(auto userentry : uh->readUsers())
	{
		User* user = &userentry.second;
		map<string, string> values
		{
		{"cardid", user->cardId},
		{"username", user->name},
		{"isallowed", user->isAllowed ? "1" : "0"},
		};
		setGroup("user", values);
	}

}

void UserSite::handleRequest(WebRequest * request)
{
	auto users = UserHandler::getInstance();
	string cardId, name, isAllowed;
	cardId = request->getArgument("editCardId");
	name = request->getArgument("editName");
	isAllowed = request->getArgument("editIsAllowed", "0");
	if(cardId.length() > 3)
		users->setUser(cardId, name, isAllowed == "1");

	WebSite::handleRequest(request);
}

void ConfigSite::fillPlaceholders()
{
}

void ConfigSite::handleRequest(WebRequest * request)
{
	WebSite::handleRequest(request);
}

void StartSite::fillPlaceholders()
{
}

void StartSite::handleRequest(WebRequest * request)
{
	auto users = UserHandler::getInstance();
	string cardId = request->getArgument("cardId");
	if(cardId.length() > 3)
	{
		users->writeLog(cardId);
	}

	WebSite::handleRequest(request);
}
