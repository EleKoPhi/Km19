#include "stdafx.h"
#include "WebSite.h"
#include "WebServer.h"

WebSite::WebSite(string targetFile) : targetFileName(targetFile)
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
		int tagend = line.find(']', tagstart);
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
		int taglen = tag.length();
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
	fillPlaceholders();

	string html = siteTemplate.generate();
	request->writeResponse(html);
}

void WebSite::fillPlaceholders()
{
	siteTemplate.childrenValues.clear();
	fillPlaceholderGroup(siteTemplate);
}

void WebSite::fillPlaceholderGroup(WebSitePlaceholderGroup& group)
{
	for(WebSitePlaceholder* child : group.childrenDefinitions)
	{
		child = child->clone();
		group.childrenValues.push_back(child);
		if(child->isGroup())
			fillPlaceholderGroup(*(WebSitePlaceholderGroup*)child);
		else
			fillPlaceholderValue(*child);
	}
}

void WebSite::fillPlaceholderValue(WebSitePlaceholder& value)
{
}


WebSitePlaceholder::WebSitePlaceholder()
{
}

WebSitePlaceholder::~WebSitePlaceholder()
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
	string output = preceding
	#ifdef _DEBUG
		+ "<!--" + name + "-->"
	#endif
		;
	//for(vector<WebSitePlaceholder*>* group : childrenValues)
	for(WebSitePlaceholder* child : childrenValues)
	{
		output = output + child->generate();
	}
#ifdef _DEBUG
	output = output + "<!--/" + name + "-->";
#endif

	return output;
}

vector<WebSitePlaceholder*>* WebSitePlaceholderGroup::addGroup()
{
	vector<WebSitePlaceholder*>* group = new vector<WebSitePlaceholder*>();
	for(WebSitePlaceholder* definition : childrenDefinitions)
		group->push_back(definition->clone());

	return group;
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
