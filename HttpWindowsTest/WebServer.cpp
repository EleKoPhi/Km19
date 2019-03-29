#include "stdafx.h"
#include "WebServer.h"
#include "WebSite.h"

WebServer::WebServer()
	: Socket(NULL)
	, index(WebSite("C:\\src\\Arduino\\Km19\\HttpWindowsTest\\Debug\\index.htm"))
	, start(WebSite("C:\\src\\Arduino\\Km19\\HttpWindowsTest\\Debug\\start.htm"))
	, users(WebSite("C:\\src\\Arduino\\Km19\\HttpWindowsTest\\Debug\\users.htm"))
	, logs(WebSite("C:\\src\\Arduino\\Km19\\HttpWindowsTest\\Debug\\log.htm"))
	, config(WebSite("C:\\src\\Arduino\\Km19\\HttpWindowsTest\\Debug\\config.htm"))
{
	index.addTarget("");
	webSites.push_back(&index);
	webSites.push_back(&start);
	webSites.push_back(&users);
	webSites.push_back(&logs);
	webSites.push_back(&config);
}


WebServer::~WebServer()
{
	// cleanup
	close();
	WSACleanup();
}


void WebServer::close(SOCKET socket)
{
	if(socket == NULL)
	{
		socket = Socket;
		Socket = NULL;
	}
	if(socket != NULL)
		closesocket(socket);
}

int WebServer::init()
{
	WSADATA wsaData;
	int iResult;

	struct addrinfo *result = NULL;
	struct addrinfo hints;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(iResult != 0)
	{
		throw exception("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	return 0;
}

inline string GetSocketError()
{
	int errorcode = WSAGetLastError();
	switch(errorcode)
	{
	case WSANOTINITIALISED: return string("WSANOTINITIALISED:\nNote  A successful WSAStartup call must occur before using this function.");
	case WSAENETDOWN: return string("WSAENETDOWN:\nThe network subsystem has failed.");
	case WSAEACCES: return string("WSAEACCES:\nAn attempt was made to access a socket in a way forbidden by its access permissions.\nThis error is returned if nn attempt to bind a datagram socket to the broadcast address failed because the setsockopt option SO_BROADCAST is not enabled.");
	case WSAEADDRINUSE: return string("WSAEADDRINUSE:\nOnly one usage of each socket address (protocol/network address/port) is normally permitted.\nThis error is returned if a process on the computer is already bound to the same fully qualified address and the socket has not been marked to allow address reuse with SO_REUSEADDR. For example, the IP address and port specified in the name parameter are already bound to another socket being used by another application. For more information, see the SO_REUSEADDR socket option in the SOL_SOCKET Socket Options reference, Using SO_REUSEADDR and SO_EXCLUSIVEADDRUSE, and SO_EXCLUSIVEADDRUSE.");
	case WSAEADDRNOTAVAIL: return string("WSAEADDRNOTAVAIL:\nThe requested address is not valid in its context.\nThis error is returned if the specified address pointed to by the name parameter is not a valid local IP address on this computer.");
	case WSAEFAULT: return string("WSAEFAULT:\nThe system detected an invalid pointer address in attempting to use a pointer argument in a call.\nThis error is returned if the name parameter is NULL, the name or namelen parameter is not a valid part of the user address space, the namelen parameter is too small, the name parameter contains an incorrect address format for the associated address family, or the first two bytes of the memory block specified by name do not match the address family associated with the socket descriptor s.");
	case WSAEINPROGRESS: return string("WSAEINPROGRESS:\nA blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	case WSAEINVAL: return string("WSAEINVAL:\nAn invalid argument was supplied.\nThis error is returned of the socket s is already bound to an address.");
	case WSAENOBUFS: return string("WSAENOBUFS:\nAn operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.\nThis error is returned of not enough buffers are available or there are too many connections.");
	case WSAENOTSOCK: return string("WSAENOTSOCK:\nAn operation was attempted on something that is not a socket.\nThis error is returned if the descriptor in the s parameter is not a socket.");
	case WSAEISCONN: return string("WSAEISCONN:\nSocket is already connected.");
	default: return string(std::to_string(errorcode) + " unknown error!");
	}
}

void WebServer::CheckErrors(int errorcode, const string& errortext)
{
	if(errorcode < 0)
	{
		string socketerror = GetSocketError();
		close();
		throw exception((string(std::to_string(errorcode) + " " + errortext + " FAILED!\n") + socketerror + "\n").c_str());
	}
	else
		printf((string(errortext) + " OK.\n").c_str());
}

void WebServer::run()
{
	struct sockaddr_in destination;

	destination.sin_family = AF_INET;

	destination.sin_port = htons(80);
	destination.sin_addr.s_addr = INADDR_ANY;

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	CheckErrors(Socket, "\nSocket Creation");

	CheckErrors(bind(Socket, (struct sockaddr *)&destination, sizeof(destination)), "Binding Socket");

	while(true)
	{
		printf("\nListening on 80...");
		CheckErrors(listen(Socket, 5), "Listening on Socket");

		struct sockaddr_in clientAddress;
		int clientSize = sizeof(clientAddress);
		SOCKET client = accept(Socket, (struct sockaddr *)&clientAddress, (int *)&clientSize);
		CheckErrors(client, "Socket Connection");
		printf("\nConnection Established!\n");

		WebRequest request(this, client);
		request.processRequest();

		close(client);
		Sleep(100);
	}
	close();
}

string WebRequest::readRequest()
{
	const int BUFFERSIZE = 2048;
	char buffer[BUFFERSIZE];
	int newData;
	newData = recv(client, buffer, BUFFERSIZE, 0);
	if(newData <= 0)
		return string("");
	return string(buffer, newData);
}

void WebRequest::parseRequest(const string& request)
{
	int start = request.find_first_of(' ');
	method = request.substr(0, start);
	log("method: '" + method + "'");
	int end = request.find_first_of(' ', ++start);
	string target = request.substr(++start, end - start);
	log("target: '" + target + "'");

	start = target.find_first_of('?');
	string args;
	if(start < 0)
	{
		args = "";
		target = target;
	}
	else
	{
		args = target.substr(start + 1);
		target = target.substr(0, start);
	}
	this->target = target;
	log("site: '" + target + "', args: '" + args + "'");

	int eq, next = 0;
	arguments.clear();
	while(next >= 0 && (eq = args.find_first_of('=', next)) > 0)
	{
		if(args[next] == '&' || args[next] == '?') next++;
		string name = args.substr(next, eq - next);
		next = args.find_first_of('&', eq++);
		string value = args.substr(eq, next - eq);
		//replace + with spaces
		for(int i = 0; (i = value.find('+', i)) >= 0; i++)
			value.replace(i, 1, " ");

		verbose("\t" + name + "='" + value + "'");
		arguments[name] = value;
	}
}

WebRequest::~WebRequest()
{
	arguments.clear();
}

void WebRequest::processRequest()
{
	string request = readRequest();
	verbose("<<");
	verbose(request);
	verbose("<</");
	if(request.length() <= 0)
		return;
	parseRequest(request);

	log("\n");
	try
	{
		for(WebSite* site : server->webSites)
		{
			if(site->canHandle(this))
			{
				log("site.canHandle: " + site->targetFileName);

				site->handleRequest(this);
				return;
			}
		}
		writeResponse("404 File not found!", "404 File not found");
	}
	catch(exception exc)
	{
		writeResponse("500 server error<br/>\n exception: " + string(exc.what()), "500 internal server error");
		return;
	}
}

void WebRequest::writeResponse(string content, string statusCodeAndText)
{

	std::string header = "HTTP/1.1 " + statusCodeAndText + " " + string("\n");
	header = header + "Server: Arduino\n";
	header = header + "Content-Length: " + to_string(content.length()) + "\n";
	header = header + "Content-Type: text/html\n\n";

	header = header + content;
	verbose(">>");
	verbose(header);
	verbose(">>/");
	send(client, header.c_str(), header.length(), 0);
}
