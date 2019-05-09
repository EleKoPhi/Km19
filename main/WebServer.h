#pragma once
#include <map>
#include <vector>
using namespace std;

#include "WebSite.h"
#include "WebSiteImplementations.h"

#include <SPI.h>
#include <WiFi101.h>

const int ListeningPort
#ifndef ARDUINO
= 8888;
#else
= 80;
#endif

class WebServer
{
	friend class WebRequest;
	friend class WebSite;

	static std::map<string, string> escapeCharacters;
#ifndef ARDUINO
	SOCKET Socket;
	void close(SOCKET socket = NULL);
	void CheckErrors(int errorcode, const string& errortext);
#else
	WiFiServer server;
	wl_status_t status;
#endif

	vector<WebSite*> webSites;

	WebSite index;
	StartSite start;
	UserSite users;
	LogSite logs;
	ConfigSite config;

	void log(string str)
	{
	#ifndef ARDUINO
		printf((str + "\n").c_str());
	#else
		Serial.println(str.c_str());
	#endif
	}
public:
	WebServer();
	~WebServer();

	int init();
	void run();

	void decode(string& text);
};

class WebRequest
{
#ifndef ARDUINO
	SOCKET client;
#else
	WiFiClient client;
#endif
	WebServer* server;
	string readRequest();
	void parseRequest(const string& request);
	void log(string str)
	{
	#ifndef ARDUINO
		printf((str + "\n").c_str());
	#else
		Serial.println(str.c_str());
	#endif
	}
public:
	string target;
	string method;
	std::map<string, string> arguments;
	string getArgument(const string& key, string fallback = "");

#ifndef ARDUINO
	WebRequest(WebServer* server
			   , SOCKET socket)
		: client(socket)
		, server(server)
	{
	}
#else
	WebRequest(WebServer* server
			   , WiFiClient client)
		: client(client)
		, server(server)
	{
	}
#endif
	~WebRequest();

	void processRequest();
	void writeResponse(string content, string statusCodeAndText = "200 OK", string contentType = "text/html; charset=ISO-8859-1", string additionalHeader = "");
	void decode(string& text) { server->decode(text); }
};

