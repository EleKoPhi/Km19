#include "stdafx.h"
#include "WebSiteImplementations.h"
#include "UserHandler.h"
#include "WebServer.h"
#include "FileIO.h"
#include <filesystem>

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
	auto uh = UserHandler::getInstance();
	string action = request->getArgument("action");
	string folder = TestFolder;

	string filename = "";
	if(action == "downloadAndReset")
	{
		filename = TimeStamp::now().getTimestamp() + "_" + to_string(uh->numberOfLogEntries()) + "_log.csv";
	#ifdef ARDUINO
		renameFile(LogFile, filename);
	#else
		if(experimental::filesystem::exists(folder + LogFile))
			experimental::filesystem::rename(folder + LogFile, folder + filename);
		else
			filename = "";
	#endif
	}
	else if(action == "downloadFile")
	{
		filename = request->getArgument("file");
	}
	if(filename != "")
	{
		string content;
		{
			content = LogEntry::csvHeader();
			map<string, User> users = uh->readUsers();
			for(auto entry : uh->readLog(UINT_MAX, filename))
			{
				string name;
				auto namelookup = users.find(entry.cardId);
				if(namelookup != users.end())
					name = namelookup->second.name;
				content.append(entry.toCsv(name));
			}
		}
		request->writeResponse(content, "200 OK", "application/octet-stream", "Content-Disposition: attachment; filename=\"" + filename + "\"");
		return;
	}
	if(!experimental::filesystem::exists(folder + LogFile))
	{
		FileWriter writer(LogFile, FileMode::Write);
		writer.write("");
	}
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
	auto uh = UserHandler::getInstance();
	map<string, string> values;
	for(auto param : uh->getParameters())
	{
		values["parameter"] = param.name;
		values["value"] = param.value;
		setGroup("entry", values);
	}
}

void ConfigSite::handleRequest(WebRequest * request)
{
	string parameter, svalue;
	parameter = request->getArgument("parameter");
	svalue = request->getArgument("value");
	if(parameter != "" && svalue != "")
	{
		try
		{
			double value = stod(svalue);
			auto uh = UserHandler::getInstance();
			uh->setParameter(parameter, value);
		}
		catch(exception exc)
		{
			log(exc.what());
		}
	}

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
