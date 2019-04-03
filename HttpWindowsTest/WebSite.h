#pragma once
#include "WebSitePlaceholders.h"
#include <set>
using namespace std;

class WebSite
{
	void log(string str) { printf((str + "\n").c_str()); }
	void verbose(string str) { printf((str + "\n").c_str()); }

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
	void setValues(const map<string, string>& values);
	void setGroup(const string& groupname, const map<string, string>& values);

public:
	WebSite(string targetFile);
	~WebSite();
	WebSite* addTarget(const string& matchingTarget);
	virtual bool canHandle(const WebRequest* request);
	virtual void handleRequest(WebRequest* request);
};
