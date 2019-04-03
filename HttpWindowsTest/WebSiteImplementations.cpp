#include "stdafx.h"
#include "WebSiteImplementations.h"

void LogSite::fillPlaceholders()
{
	{
		map<string, string> values{{"currententries", "99"}};
		setValues(values);
	}
	{
		map<string, string> entry
		{
			{"timestamp", "2019-04-02 13:37"},
		{"cardid", "xyz"},
		{"username", "Werner Schunn"},
		{"type", "1"},
		};
		setGroup("entry", entry);
	}
	{
		map<string, string> entry
		{
			{"timestamp", "2019-04-02 13:39"},
		{"cardid", "abc"},
		{"username", "Philipp Mochti"},
		{"type", "2"},
		};
		setGroup("entry", entry);
	}
	{
		map<string, string> file
		{
			{"filename", "20190331_1339_123entries.log"},
		};
		setGroup("file", file);
	}
}

void UserSite::fillPlaceholders()
{
	{
		map<string, string> user
		{
			{"cardid", "xyz"},
		{"username", "Werner Schunn"},
		};
		setGroup("user", user);
	}
	{
		map<string, string> user
		{
			{"cardid", "abc"},
		{"username", "Philipp Mochti"},
		};
		setGroup("user", user);
	}

}

void ConfigSite::fillPlaceholders()
{
}

void StartSite::fillPlaceholders()
{
}
