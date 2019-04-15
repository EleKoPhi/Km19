#include "stdafx.h"
#include "WebSitePlaceholders.h"


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

