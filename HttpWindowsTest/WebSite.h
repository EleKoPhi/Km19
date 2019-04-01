#pragma once

#include <set>
#include <vector>
using namespace std;

class WebRequest;
class WebSitePlaceholderGroup;

class WebSitePlaceholder
{
	friend class WebSite;
	friend class WebSitePlaceholderGroup;

protected:
	void log(string str) { printf((str + "\n").c_str()); }
	void verbose(string str) { printf((str + "\n").c_str()); }
	virtual WebSitePlaceholderGroup* asGroup() { return NULL; }

public:
	string preceding;
	string name;
	string value;
	virtual string generate();

	WebSitePlaceholder* clone();

	WebSitePlaceholder();
	~WebSitePlaceholder();
	virtual bool isGroup() { return false; }
};

class WebSitePlaceholderGroup : public WebSitePlaceholder
{
	friend class WebSite;
	WebSitePlaceholder* addValueDefinition(string name, string preceding);
	WebSitePlaceholderGroup* addGroupDefinition(string name, string preceding);
	WebSitePlaceholderGroup* parentGroup;
	void clearValues();
protected:
	virtual WebSitePlaceholderGroup* asGroup() { return this; }

public:
	WebSitePlaceholderGroup();
	~WebSitePlaceholderGroup();
	vector<WebSitePlaceholder*> childrenDefinitions;
	vector<WebSitePlaceholder*> childrenValues;
	virtual string generate();

	vector<WebSitePlaceholder*>* addGroup();
	virtual bool isGroup() { return true; }
};

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
protected:
	void fillPlaceholders();
	virtual void fillPlaceholderGroup(WebSitePlaceholderGroup& group);
	virtual void fillPlaceholderValue(WebSitePlaceholder& value);

public:
	WebSite(string targetFile);
	~WebSite();
	WebSite* addTarget(const string& matchingTarget);
	virtual bool canHandle(const WebRequest* request);
	virtual void handleRequest(WebRequest* request);
};
