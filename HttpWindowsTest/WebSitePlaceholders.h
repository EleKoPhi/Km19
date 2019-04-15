#pragma once

#include <vector>
#include <map>
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
	string name;
	string value;
	string preceding;
	virtual string generate();

	virtual WebSitePlaceholder* clone();

	WebSitePlaceholder();
	~WebSitePlaceholder();
	virtual bool isGroup() { return false; }
	virtual void prepare();
};

class WebSitePlaceholderGroup : public WebSitePlaceholder
{
	bool isDummy() { return parentGroup != NULL && parentGroup->name != name; }
	friend class WebSite;
	WebSitePlaceholder* addValueDefinition(string name, string preceding);
	WebSitePlaceholderGroup* addGroupDefinition(string name, string preceding);
	WebSitePlaceholderGroup* parentGroup;
	void clearValues();
	virtual WebSitePlaceholder* clone();
	void cloneInternal(WebSitePlaceholderGroup* clone);
	bool insertGroup(WebSitePlaceholderGroup& group);

protected:
	virtual WebSitePlaceholderGroup* asGroup() { return this; }

public:
	WebSitePlaceholderGroup();
	~WebSitePlaceholderGroup();
	vector<WebSitePlaceholder*> childrenDefinitions;
	vector<WebSitePlaceholder*> childrenValues;
	virtual string generate();

	WebSitePlaceholderGroup* addGroup();
	virtual bool isGroup() { return true; }

	void setValues(const map<string, string>& values);
	void setGroup(const string& groupname, const map<string, string>& values);
	virtual void prepare();
};
