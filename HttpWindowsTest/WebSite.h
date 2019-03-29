#pragma once

#include <set>
using namespace std;

class WebRequest;

class WebSite
{
	friend class WebServer;
	friend class WebRequest;

	string targetFileName;
	string targetHtml;
	set<string> matchingTargets;
	void loadFile();
public:
	WebSite(string targetFile);
	~WebSite();
	WebSite* addTarget(const string& matchingTarget);
	virtual bool canHandle(const WebRequest* request);
	virtual void handleRequest(WebRequest* request);
};

