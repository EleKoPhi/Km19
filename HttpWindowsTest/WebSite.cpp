#include "stdafx.h"
#include "WebSite.h"
#include "WebServer.h"

WebSite::WebSite(string targetFile)
	: targetFileName(targetFile)
	, targetHtml("")
{
	int i = targetFile.find_last_of('\\');
	if(i < 0) i = targetFile.find_last_of('/');
	if(i >= 0) targetFile = targetFile.substr(i + 1);
	addTarget(targetFile);

	i = targetFile.find_last_of('.');
	if(i >= 0) targetFile = targetFile.substr(0, i);
	addTarget(targetFile);

	loadFile();
}

void WebSite::loadFile()
{
	ifstream target;
	target.open(targetFileName);

	string line;
	WebSitePlaceholderGroup* masterGroup = &siteTemplate;
	WebSitePlaceholderGroup* currentGroup = masterGroup;
	targetHtml = "";
	if(target.is_open())
		while(getline(target, line))
		{
			if(!parseTags(&currentGroup, targetHtml, line))
				targetHtml = targetHtml + line + "\n";
		}
	siteTemplate.addValueDefinition("", targetHtml);
	target.close();
}

bool WebSite::parseTags(WebSitePlaceholderGroup** group, string& fullHtml, string& line)
{
	int tagstart;
	if((tagstart = line.find('[')) >= 4)
	{// line might have a template-tag...
		if(line[tagstart - 1] != '-'
		   || line[tagstart - 2] != '-'
		   || line[tagstart - 3] != '!'
		   || line[tagstart - 4] != '<')
			return false;
		size_t tagend = line.find(']', tagstart);
		if(tagend < 0)
			return false;

		if(line[tagend + 1] != '-'
		   || line[tagend + 2] != '-'
		   || line[tagend + 3] != '>')
		{
			log("looks like a opened tag but was not closed: " + line);
			return false;
		}

		string tag = line.substr(tagstart + 1, tagend - tagstart - 1);
		size_t taglen = tag.length();
		bool isGroupStartTag = false;
		bool isGroupEndTag = false;
		for(size_t i = 0; i < taglen; i++)
		{
			char c = tag[i];
			if((isGroupStartTag = (i == taglen - 1 && c == ':'))
			   || (isGroupEndTag |= (i == 0 && c == '/'))
			   || (c >= '0' && c <= '9')
			   || (c >= 'A' && c <= 'Z')
			   || (c >= 'a' && c <= 'z')
			   )
			{
			}
			else
			{
				log("'" + tag + "' is not a tag because of '" + tag[i] + "'@" + to_string(i));
				return false;
			}
		}

		string preceding = fullHtml + line.substr(0, tagstart - 4);
		if(isGroupEndTag)
		{
			(*group)->addValueDefinition("", preceding);

			(*group) = (*group)->parentGroup;
		}
		else if(isGroupStartTag)
		{
			auto newgroup = (*group)->addGroupDefinition(tag.substr(0, tag.length() - 1), preceding);
			newgroup->parentGroup = *group;
			(*group) = newgroup;
		}
		else
		{
			(*group)->addValueDefinition(tag, preceding);
		}
		if(line.length() < tagend + 4)
			fullHtml = "\n";
		else
			fullHtml = line.substr(tagend + 4) + "\n";

		return true;
	}
	return false;
}

WebSite::~WebSite()
{
	matchingTargets.clear();
}

WebSite* WebSite::addTarget(const string& matchingTarget)
{
	matchingTargets.insert(matchingTarget);
	return this;
}

bool WebSite::canHandle(const WebRequest* request)
{
	if(matchingTargets.find(request->target) != matchingTargets.end())
		return true;
	return false;
}
void WebSite::handleRequest(WebRequest * request)
{
	processPlaceholders();

	string html = siteTemplate.generate();
	request->writeResponse(html);
}

void WebSite::processPlaceholders()
{
	siteTemplate.childrenValues.clear();
	siteTemplate.prepare();

	fillPlaceholders();
}

void WebSite::fillPlaceholders()
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

void WebSite::setValues(const map<string, string>& values)
{
	siteTemplate.setValues(values);
}

void WebSite::setGroup(const string & groupname, const map<string, string>& values)
{
	siteTemplate.setGroup(groupname, values);
}


WebSitePlaceholder::WebSitePlaceholder()
	: name("")
	, value("")
	, preceding("")
{
}

WebSitePlaceholder::~WebSitePlaceholder()
{
}

void WebSitePlaceholder::prepare()
{
}

string WebSitePlaceholder::generate()
{
	return preceding
	#ifdef _DEBUG
		+ "<!--" + name + "-->"
	#endif
		+ value;
}

WebSitePlaceholder * WebSitePlaceholder::clone()
{
	WebSitePlaceholder* clone = new WebSitePlaceholder();
	clone->name = name;
	clone->preceding = preceding;
	return clone;
}

WebSitePlaceholderGroup::WebSitePlaceholderGroup()
	: parentGroup(NULL)
{
}

WebSitePlaceholderGroup::~WebSitePlaceholderGroup()
{
	for(WebSitePlaceholder* child : childrenDefinitions)
		delete child;
	clearValues();
	childrenDefinitions.clear();
}

string WebSitePlaceholderGroup::generate()
{
	string output = "";
	if(isDummy())
		output = output + preceding;
#ifdef _DEBUG
	output = output + "\n<!--" + name + "-->\n";
#endif

	//for(vector<WebSitePlaceholder*>* group : childrenValues)
	for(WebSitePlaceholder* child : childrenValues)
	{
		output = output + child->generate();
	}
#ifdef _DEBUG
	output = output + "\n<!--/" + name + "-->\n";
#endif

	return output;
}

WebSitePlaceholderGroup* WebSitePlaceholderGroup::addGroup()
{
	WebSitePlaceholderGroup* group = this->clone()->asGroup();
	group->parentGroup = this;
	group->prepare();

	//if(!this->insertGroup(*group))
	this->childrenValues.push_back(group);


	return group;
}

bool WebSitePlaceholderGroup::insertGroup(WebSitePlaceholderGroup & group)
{
	//for(bool retry = true; retry == true; )
	//{
	//	retry = false;

	for(auto it = this->childrenValues.begin(); it != this->childrenValues.end(); it++)
	{
		auto val = *it;
		if(val->name != this->name)
		{
			this->childrenValues.insert(it, &group);
			return true;
		}
		//else
		//	if(val->asGroup()->isDummy)
		//	{
		//		parentGroup->childrenValues.erase(it);
		//		retry = true;
		//		break;
		//	}
	}
	//}
	return false;
}
void WebSitePlaceholderGroup::setValues(const map<string, string>& values)
{
	for(auto child : childrenValues)
	{
		if(child->name == "")
			continue;
		if(child->isGroup())
			continue;
		auto it = values.find(child->name);
		if(it != values.end())
		{
			string value = it->second;
			child->value = value;

		}
	}
}

void WebSitePlaceholderGroup::setGroup(const string & groupname, const map<string, string>& values)
{
	for(auto child : childrenValues)
	{
		if(child->name == "")
			continue;
		if(!child->isGroup())
			continue;
		if(child->name != groupname)
			continue;
		auto group = child->asGroup()->addGroup();
		group->setValues(values);
		return;
	}
	log("cannot set group '" + groupname + "': not found.");
}

void WebSitePlaceholderGroup::prepare()
{
	if(this->isDummy())
		return;
	for(WebSitePlaceholder* child : childrenDefinitions)
	{
		child = child->clone();
		childrenValues.push_back(child);
		if(child->isGroup())
			child->asGroup()->prepare();
		else
			child->prepare();
	}
}

WebSitePlaceholder * WebSitePlaceholderGroup::addValueDefinition(string name, string preceding)
{
	WebSitePlaceholder* ph = new WebSitePlaceholder();
	childrenDefinitions.push_back(ph);
	ph->name = name;
	ph->preceding = preceding;
	return ph;
}

WebSitePlaceholderGroup * WebSitePlaceholderGroup::addGroupDefinition(string name, string preceding)
{
	WebSitePlaceholderGroup* ph = new WebSitePlaceholderGroup();
	childrenDefinitions.push_back(ph);
	ph->name = name;
	ph->preceding = preceding;
	return ph;
}

void WebSitePlaceholderGroup::clearValues()
{
	for(auto child : childrenValues)
	{
		if(child->isGroup())
			child->asGroup()->clearValues();
		delete child;
	}
	childrenValues.clear();
}

WebSitePlaceholder * WebSitePlaceholderGroup::clone()
{
	WebSitePlaceholderGroup* clone = new WebSitePlaceholderGroup();
	clone->preceding = preceding;
	clone->parentGroup = parentGroup;
	cloneInternal(clone);
	return clone;
}

void WebSitePlaceholderGroup::cloneInternal(WebSitePlaceholderGroup * clone)
{
	clone->name = name;
	for(auto child : childrenDefinitions)
	{
		clone->childrenDefinitions.push_back(child->clone());
	}
}

