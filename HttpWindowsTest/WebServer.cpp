#include "stdafx.h"
#include "WebServer.h"
#include "WebSite.h"

map<string, string> escapeCharacters
{
	{"%20", " "},
{"%21", "!"},
{"%22", "\"" },
{ "%23", "#" },
{"%24", "$"},
{"%25", "%"},
{"%26", "&"},
{"%27", "'"},
{"%28", "("},
{"%29", ")"},
{"%2A", "*"},
{"%2B", "+"},
{"%2C", ","},
{"%2D", "-"},
{"%2E", "."},
{"%2F", "/"},
{"%30", "0"},
{"%31", "1"},
{"%32", "2"},
{"%33", "3"},
{"%34", "4"},
{"%35", "5"},
{"%36", "6"},
{"%37", "7"},
{"%38", "8"},
{"%39", "9"},
{"%3A", ":"},
{"%3B", ";"},
{"%3C", "<"},
{"%3D", "="},
{"%3E", ">"},
{"%3F", "?"},
{"%40", "@"},
{"%41", "A"},
{"%42", "B"},
{"%43", "C"},
{"%44", "D"},
{"%45", "E"},
{"%46", "F"},
{"%47", "G"},
{"%48", "H"},
{"%49", "I"},
{"%4A", "J"},
{"%4B", "K"},
{"%4C", "L"},
{"%4D", "M"},
{"%4E", "N"},
{"%4F", "O"},
{"%50", "P"},
{"%51", "Q"},
{"%52", "R"},
{"%53", "S"},
{"%54", "T"},
{"%55", "U"},
{"%56", "V"},
{"%57", "W"},
{"%58", "X"},
{"%59", "Y"},
{"%5A", "Z"},
{"%5B", "["},
{"%5C", "\\" },
{ "%5D", "]" },
{"%5E", "^"},
{"%5F", "_"},
{"%60", "`"},
{"%61", "a"},
{"%62", "b"},
{"%63", "c"},
{"%64", "d"},
{"%65", "e"},
{"%66", "f"},
{"%67", "g"},
{"%68", "h"},
{"%69", "i"},
{"%6A", "j"},
{"%6B", "k"},
{"%6C", "l"},
{"%6D", "m"},
{"%6E", "n"},
{"%6F", "o"},
{"%70", "p"},
{"%71", "q"},
{"%72", "r"},
{"%73", "s"},
{"%74", "t"},
{"%75", "u"},
{"%76", "v"},
{"%77", "w"},
{"%78", "x"},
{"%79", "y"},
{"%7A", "z"},
{"%7B", "{"},
{"%7C", "|"},
{"%7D", "}"},
{"%7E", "~"},
{"%7F", "%7F"},
{"%80", "`"},
{"%81", "Å"},
{"%82", "Ç"},
{"%83", "É"},
{"%84", "Ñ"},
{"%85", "Ö"},
{"%86", "Ü"},
{"%87", "á"},
{"%88", "à"},
{"%89", "â"},
{"%8A", "ä"},
{"%8B", "ã"},
{"%8C", "å"},
{"%8D", "ç"},
{"%8E", "é"},
{"%8F", "è"},
{"%90", "ê"},
{"%91", "ë"},
{"%92", "í"},
{"%93", "ì"},
{"%94", "î"},
{"%95", "ï"},
{"%96", "ñ"},
{"%97", "ó"},
{"%98", "ò"},
{"%99", "ô"},
{"%9A", "ö"},
{"%9B", "õ"},
{"%9C", "ú"},
{"%9D", "ù"},
{"%9E", "û"},
{"%9F", "ü"},
{"%C2", "%A0"},
{"%A1", "°"},
{"%A2", "¢"},
{"%A3", "£"},
{"%A4", "§"},
{"%A5", "•"},
{"%A6", "¶"},
{"%A7", "ß"},
{"%A8", "®"},
{"%A9", "©"},
{"%AA", "™"},
{"%AB", "´"},
{"%AC", "¨"},
{"%AD", "≠"},
{"%AE", "Æ"},
{"%AF", "Ø"},
{"%B0", "∞"},
{"%B1", "±"},
{"%B2", "≤"},
{"%B3", "≥"},
{"%B4", "¥"},
{"%B5", "µ"},
{"%B6", "∂"},
{"%B7", "∑"},
{"%B8", "∏"},
{"%B9", "π"},
{"%BA", "∫"},
{"%BB", "ª"},
{"%BC", "º"},
{"%BD", "Ω"},
{"%BE", "æ"},
{"%BF", "ø"},
{"%C0", "¿"},
{"%C1", "¡"},
{"%C2", "¬"},
{"%C3", "√"},
{"%C4", "ƒ"},
{"%C5", "≈"},
{"%C6", "∆"},
{"%C7", "«"},
{"%C8", "»"},
{"%C9", "…"},
{"%CA", " "},
{"%CB", "À"},
{"%CC", "Ã"},
{"%CD", "Õ"},
{"%CE", "Œ"},
{"%CF", "œ"},
{"%D0", "–"},
{"%D1", "—"},
{"%D2", "“"},
{"%D3", "”"},
{"%D4", "‘"},
{"%D5", "’"},
{"%D6", "÷"},
{"%D7", "◊"},
{"%D8", "ÿ"},
{"%D9", "Ÿ"},
{"%DA", "⁄"},
{"%DB", "€"},
{"%DC", "‹"},
{"%DD", "›"},
{"%DE", "ﬁ"},
{"%DF", "ﬂ"},
{"%E0", "‡"},
{"%E1", "·"},
{"%E2", "‚"},
{"%E3", "„"},
{"%E4", "‰"},
{"%E5", "Â"},
{"%E6", "Ê"},
{"%E7", "Á"},
{"%E8", "Ë"},
{"%E9", "È"},
{"%EA", "Í"},
{"%EB", "Î"},
{"%EC", "Ï"},
{"%ED", "Ì"},
{"%EE", "Ó"},
{"%EF", "Ô"},
{"%F0", ""},
{"%F1", "Ò"},
{"%F2", "Ú"},
{"%F3", "Û"},
{"%F4", "Ù"},
{"%F5", "ı"},
{"%F6", "ˆ"},
{"%F7", "˜"},
{"%F8", "¯"},
{"%F9", "˘"},
{"%FA", "˙"},
{"%FB", "˚"},
{"%FC", "¸"},
{"%FD", "˝"},
{"%FE", "˛"},
};

WebServer::WebServer()
	: Socket(NULL)
	, index("index.htm")
	, start()
	, users()
	, logs()
	, config()
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

	destination.sin_port = htons(ListeningPort);
	destination.sin_addr.s_addr = INADDR_ANY;

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	CheckErrors(Socket, "\nSocket Creation");

	CheckErrors(bind(Socket, (struct sockaddr *)&destination, sizeof(destination)), "Binding Socket");

	while(true)
	{
		log("\nListening on " + to_string(ListeningPort) + "...");
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

void WebServer::decode(string& text)
{
	size_t pos = -1;
	while((pos = text.find('%', ++pos)) != string::npos)
	{
		if(pos + 3 >= text.length())
			continue;
		string ch = text.substr(pos, 3);
		auto res = escapeCharacters.find(ch);
		if(res == escapeCharacters.end())
			continue;
		text.replace(pos, ch.length(), res->second);
	}
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
	string target = "";
	if(++start < end)
		target = request.substr(start, end - start);
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
		decode(value);
		arguments[name] = value;
	}
}

string WebRequest::getArgument(const string & key, string fallback)
{
	auto result = arguments.find(key);
	if(result != arguments.end())
		return result->second;
	return
		fallback;
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
		writeResponse("404 File not found!", "404 not found");
	}
	catch(exception exc)
	{
		writeResponse("500 internal server error<br/>\n exception: " + string(exc.what()), "500 internal server error");
		return;
	}
}

void WebRequest::writeResponse(string content, string statusCodeAndText, string contentType, string additionalHeader)
{

	std::string header = "HTTP/1.1 " + statusCodeAndText + " " + string("\n");
	header = header + "Server: Arduino\n";
	header = header + "Content-Length: " + to_string(content.length()) + "\n";
	header = header + "Content-Type: " + contentType + "\n";
	header = header + additionalHeader + "\n\n";

	header = header + content;
	//verbose(">>");
	//verbose(header);
	//verbose(">>/");
	send(client, header.c_str(), header.length(), 0);
}
