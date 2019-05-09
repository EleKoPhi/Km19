#pragma once
#include "WebSitePlaceholders.h"
#include <set>
using namespace std;

class WebSite
{
protected:
	void log(string str)
	{
	#ifndef ARDUINO
		printf((str + "\n").c_str());
	#else
		Serial.println(str.c_str());
	#endif
	}

	friend class WebServer;
	friend class WebRequest;
	friend class WebSitePlaceholder;
	friend class WebSitePlaceholderGroup;

	string targetFileName;
	string targetHtml;
	set<string> matchingTargets;
	void loadFile();
	bool parseTags(WebSitePlaceholderGroup** group, string& fullHtml, string& line);
	WebSitePlaceholderGroup siteTemplate;
	void processPlaceholders();

protected:
	virtual void fillPlaceholders();
	void setValues(const std::map<string, string>& values);
	void setGroup(const string& groupname, const std::map<string, string>& values);

public:
	WebSite(string targetFile);
	~WebSite();
	WebSite* addTarget(const string& matchingTarget);
	virtual bool canHandle(const WebRequest* request);
	virtual void handleRequest(WebRequest* request);
};
