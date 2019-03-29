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
	targetHtml = "";
	if(target.is_open())
		while(getline(target, line))
			targetHtml = targetHtml + line + "\n";
	target.close();
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

void WebSite::handleRequest(WebRequest * request)
{
	request->writeResponse(targetHtml);
}

bool WebSite::canHandle(const WebRequest* request)
{
	if(matchingTargets.find(request->target) != matchingTargets.end())
		return true;
	return false;
}
