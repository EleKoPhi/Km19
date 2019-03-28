#pragma once
#include <map>
using namespace std;
class WebServer
{
	SOCKET Socket;
	void close(SOCKET socket = NULL);
	//string GetLastError();
	void CheckErrors(int errorcode, const string& errortext);

public:
	WebServer();
	~WebServer();

	int init();
	void run();

};

class WebRequest
{
	SOCKET client;
	string readRequest();
	void parseRequest(const string& request);
	string method;
	string site;
	void log(string str) { printf((str + "\n").c_str()); }
	void verbose(string str) { printf((str + "\n").c_str()); }
	map<string, string> arguments;
public:
	WebRequest(SOCKET socket) : client(socket) {}
	~WebRequest() {}

	void processRequest();

};