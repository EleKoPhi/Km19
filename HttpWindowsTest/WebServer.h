#pragma once
#include <map>
#include <vector>
using namespace std;

#include "WebSite.h"
#include "WebSiteImplementations.h"

const int ListeningPort 
#ifdef _DEBUG
= 8888; 
#else
= 80;
#endif

class WebServer
{
	friend class WebRequest;
	friend class WebSite;

	SOCKET Socket;
	void close(SOCKET socket = NULL);
	void CheckErrors(int errorcode, const string& errortext);
	
	vector<WebSite*> webSites;

	WebSite index;
	StartSite start;
	UserSite users;
	LogSite logs;
	ConfigSite config;

	void log(string str) { printf((str + "\n").c_str()); }
	void verbose(string str) { printf((str + "\n").c_str()); }
public:
	WebServer();
	~WebServer();

	int init();
	void run();

};

class WebRequest
{
	SOCKET client;
	WebServer* server;
	string readRequest();
	void parseRequest(const string& request);
	void log(string str) { printf((str + "\n").c_str()); }
	void verbose(string str) { printf((str + "\n").c_str()); }
public:
	string target;
	string method;
	map<string, string> arguments;

	WebRequest(WebServer* server, SOCKET socket) : client(socket), server(server) {}
	~WebRequest();

	void processRequest();
	void writeResponse(string content, string statusCodeAndText = "200 OK");
};