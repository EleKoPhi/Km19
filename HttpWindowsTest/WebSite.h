#pragma once
class WebRequest;

class WebSite
{
public:
	WebSite();
	~WebSite();

	virtual bool canHandle(WebRequest& request);
	virtual void handleRequest(WebRequest& request);
};

