#include "stdafx.h"
#include "WebServer.h"
#include "WebSite.h"

std::map<string, string> WebServer::escapeCharacters;

WebServer::WebServer()
	:
#ifndef ARDUINO
	Socket(NULL)
#else
	server(ListeningPort)
#endif
	, index("index.htm")
	, start()
	, users()
	, logs()
	, config()
{


	escapeCharacters["%20"] = " ";
	escapeCharacters["%21"] = "!";
	escapeCharacters["%22"] = "\"";
	escapeCharacters["%23"] = "#";
	escapeCharacters["%24"] = "$";
	escapeCharacters["%25"] = "%";
	escapeCharacters["%26"] = "&";
	escapeCharacters["%27"] = "'";
	escapeCharacters["%28"] = "(";
	escapeCharacters["%29"] = ")";
	escapeCharacters["%2A"] = "*";
	escapeCharacters["%2B"] = "+";
	escapeCharacters["%2C"] = ",";
	escapeCharacters["%2D"] = "-";
	escapeCharacters["%2E"] = ".";
	escapeCharacters["%2F"] = "/";
	escapeCharacters["%30"] = "0";
	escapeCharacters["%31"] = "1";
	escapeCharacters["%32"] = "2";
	escapeCharacters["%33"] = "3";
	escapeCharacters["%34"] = "4";
	escapeCharacters["%35"] = "5";
	escapeCharacters["%36"] = "6";
	escapeCharacters["%37"] = "7";
	escapeCharacters["%38"] = "8";
	escapeCharacters["%39"] = "9";
	escapeCharacters["%3A"] = ":";
	escapeCharacters["%3B"] = ";";
	escapeCharacters["%3C"] = "<";
	escapeCharacters["%3D"] = "=";
	escapeCharacters["%3E"] = ">";
	escapeCharacters["%3F"] = "?";
	escapeCharacters["%40"] = "@";
	escapeCharacters["%41"] = "A";
	escapeCharacters["%42"] = "B";
	escapeCharacters["%43"] = "C";
	escapeCharacters["%44"] = "D";
	escapeCharacters["%45"] = "E";
	escapeCharacters["%46"] = "F";
	escapeCharacters["%47"] = "G";
	escapeCharacters["%48"] = "H";
	escapeCharacters["%49"] = "I";
	escapeCharacters["%4A"] = "J";
	escapeCharacters["%4B"] = "K";
	escapeCharacters["%4C"] = "L";
	escapeCharacters["%4D"] = "M";
	escapeCharacters["%4E"] = "N";
	escapeCharacters["%4F"] = "O";
	escapeCharacters["%50"] = "P";
	escapeCharacters["%51"] = "Q";
	escapeCharacters["%52"] = "R";
	escapeCharacters["%53"] = "S";
	escapeCharacters["%54"] = "T";
	escapeCharacters["%55"] = "U";
	escapeCharacters["%56"] = "V";
	escapeCharacters["%57"] = "W";
	escapeCharacters["%58"] = "X";
	escapeCharacters["%59"] = "Y";
	escapeCharacters["%5A"] = "Z";
	escapeCharacters["%5B"] = "[";
	escapeCharacters["%5C"] = "\\";
	escapeCharacters["%5D"] = "]";
	escapeCharacters["%5E"] = "^";
	escapeCharacters["%5F"] = "_";
	escapeCharacters["%60"] = "`";
	escapeCharacters["%61"] = "a";
	escapeCharacters["%62"] = "b";
	escapeCharacters["%63"] = "c";
	escapeCharacters["%64"] = "d";
	escapeCharacters["%65"] = "e";
	escapeCharacters["%66"] = "f";
	escapeCharacters["%67"] = "g";
	escapeCharacters["%68"] = "h";
	escapeCharacters["%69"] = "i";
	escapeCharacters["%6A"] = "j";
	escapeCharacters["%6B"] = "k";
	escapeCharacters["%6C"] = "l";
	escapeCharacters["%6D"] = "m";
	escapeCharacters["%6E"] = "n";
	escapeCharacters["%6F"] = "o";
	escapeCharacters["%70"] = "p";
	escapeCharacters["%71"] = "q";
	escapeCharacters["%72"] = "r";
	escapeCharacters["%73"] = "s";
	escapeCharacters["%74"] = "t";
	escapeCharacters["%75"] = "u";
	escapeCharacters["%76"] = "v";
	escapeCharacters["%77"] = "w";
	escapeCharacters["%78"] = "x";
	escapeCharacters["%79"] = "y";
	escapeCharacters["%7A"] = "z";
	escapeCharacters["%7B"] = "{";
	escapeCharacters["%7C"] = "|";
	escapeCharacters["%7D"] = "}";
	escapeCharacters["%7E"] = "~";
	escapeCharacters["%7F"] = "%7F";
	escapeCharacters["%80"] = "`";
	escapeCharacters["%81"] = "Å";
	escapeCharacters["%82"] = "Ç";
	escapeCharacters["%83"] = "É";
	escapeCharacters["%84"] = "Ñ";
	escapeCharacters["%85"] = "Ö";
	escapeCharacters["%86"] = "Ü";
	escapeCharacters["%87"] = "á";
	escapeCharacters["%88"] = "à";
	escapeCharacters["%89"] = "â";
	escapeCharacters["%8A"] = "ä";
	escapeCharacters["%8B"] = "ã";
	escapeCharacters["%8C"] = "å";
	escapeCharacters["%8D"] = "ç";
	escapeCharacters["%8E"] = "é";
	escapeCharacters["%8F"] = "è";
	escapeCharacters["%90"] = "ê";
	escapeCharacters["%91"] = "ë";
	escapeCharacters["%92"] = "í";
	escapeCharacters["%93"] = "ì";
	escapeCharacters["%94"] = "î";
	escapeCharacters["%95"] = "ï";
	escapeCharacters["%96"] = "ñ";
	escapeCharacters["%97"] = "ó";
	escapeCharacters["%98"] = "ò";
	escapeCharacters["%99"] = "ô";
	escapeCharacters["%9A"] = "ö";
	escapeCharacters["%9B"] = "õ";
	escapeCharacters["%9C"] = "ú";
	escapeCharacters["%9D"] = "ù";
	escapeCharacters["%9E"] = "û";
	escapeCharacters["%9F"] = "ü";
	escapeCharacters["%C2"] = "%A0";
	escapeCharacters["%A1"] = "°";
	escapeCharacters["%A2"] = "¢";
	escapeCharacters["%A3"] = "£";
	escapeCharacters["%A4"] = "§";
	escapeCharacters["%A5"] = "•";
	escapeCharacters["%A6"] = "¶";
	escapeCharacters["%A7"] = "ß";
	escapeCharacters["%A8"] = "®";
	escapeCharacters["%A9"] = "©";
	escapeCharacters["%AA"] = "™";
	escapeCharacters["%AB"] = "´";
	escapeCharacters["%AC"] = "¨";
	escapeCharacters["%AD"] = "≠";
	escapeCharacters["%AE"] = "Æ";
	escapeCharacters["%AF"] = "Ø";
	escapeCharacters["%B0"] = "∞";
	escapeCharacters["%B1"] = "±";
	escapeCharacters["%B2"] = "≤";
	escapeCharacters["%B3"] = "≥";
	escapeCharacters["%B4"] = "¥";
	escapeCharacters["%B5"] = "µ";
	escapeCharacters["%B6"] = "∂";
	escapeCharacters["%B7"] = "∑";
	escapeCharacters["%B8"] = "∏";
	escapeCharacters["%B9"] = "π";
	escapeCharacters["%BA"] = "∫";
	escapeCharacters["%BB"] = "ª";
	escapeCharacters["%BC"] = "º";
	escapeCharacters["%BD"] = "Ω";
	escapeCharacters["%BE"] = "æ";
	escapeCharacters["%BF"] = "ø";
	escapeCharacters["%C0"] = "¿";
	escapeCharacters["%C1"] = "¡";
	escapeCharacters["%C2"] = "¬";
	escapeCharacters["%C3"] = "√";
	escapeCharacters["%C4"] = "ƒ";
	escapeCharacters["%C5"] = "≈";
	escapeCharacters["%C6"] = "∆";
	escapeCharacters["%C7"] = "«";
	escapeCharacters["%C8"] = "»";
	escapeCharacters["%C9"] = "…";
	escapeCharacters["%CA"] = " ";
	escapeCharacters["%CB"] = "À";
	escapeCharacters["%CC"] = "Ã";
	escapeCharacters["%CD"] = "Õ";
	escapeCharacters["%CE"] = "Œ";
	escapeCharacters["%CF"] = "œ";
	escapeCharacters["%D0"] = "–";
	escapeCharacters["%D1"] = "—";
	escapeCharacters["%D2"] = "“";
	escapeCharacters["%D3"] = "”";
	escapeCharacters["%D4"] = "‘";
	escapeCharacters["%D5"] = "’";
	escapeCharacters["%D6"] = "÷";
	escapeCharacters["%D7"] = "◊";
	escapeCharacters["%D8"] = "ÿ";
	escapeCharacters["%D9"] = "Ÿ";
	escapeCharacters["%DA"] = "⁄";
	escapeCharacters["%DB"] = "€";
	escapeCharacters["%DC"] = "‹";
	escapeCharacters["%DD"] = "›";
	escapeCharacters["%DE"] = "ﬁ";
	escapeCharacters["%DF"] = "ﬂ";
	escapeCharacters["%E0"] = "‡";
	escapeCharacters["%E1"] = "·";
	escapeCharacters["%E2"] = "‚";
	escapeCharacters["%E3"] = "„";
	escapeCharacters["%E4"] = "‰";
	escapeCharacters["%E5"] = "Â";
	escapeCharacters["%E6"] = "Ê";
	escapeCharacters["%E7"] = "Á";
	escapeCharacters["%E8"] = "Ë";
	escapeCharacters["%E9"] = "È";
	escapeCharacters["%EA"] = "Í";
	escapeCharacters["%EB"] = "Î";
	escapeCharacters["%EC"] = "Ï";
	escapeCharacters["%ED"] = "Ì";
	escapeCharacters["%EE"] = "Ó";
	escapeCharacters["%EF"] = "Ô";
	escapeCharacters["%F0"] = "";
	escapeCharacters["%F1"] = "Ò";
	escapeCharacters["%F2"] = "Ú";
	escapeCharacters["%F3"] = "Û";
	escapeCharacters["%F4"] = "Ù";
	escapeCharacters["%F5"] = "ı";
	escapeCharacters["%F6"] = "ˆ";
	escapeCharacters["%F7"] = "˜";
	escapeCharacters["%F8"] = "¯";
	escapeCharacters["%F9"] = "˘";
	escapeCharacters["%FA"] = "˙";
	escapeCharacters["%FB"] = "˚";
	escapeCharacters["%FC"] = "¸";
	escapeCharacters["%FD"] = "˝";
	escapeCharacters["%FE"] = "˛";

	index.addTarget("");
	webSites.push_back(&index);
	webSites.push_back(&start);
	webSites.push_back(&users);
	webSites.push_back(&logs);
	webSites.push_back(&config);
}


WebServer::~WebServer()
{
#ifndef ARDUINO
	// cleanup
	close();
	WSACleanup();
#endif // !ARDUINO
}

#ifndef ARDUINO
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
		throw Exception((string(to_string(errorcode) + " " + errortext + " FAILED!\n") + socketerror + "\n").c_str());
	}
	else
		log((string(errortext) + " OK.\n").c_str());
}


#endif // !ARDUINO

int WebServer::init()
{
#ifndef ARDUINO
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
#else
	if(WiFi.status() == WL_NO_SHIELD)
	{
		log("WiFi shield not present");
		throw Exception("WiFi shield not present");
		return 1;
	}

	return 0;
#endif
}

void WebServer::run()
{
#ifndef ARDUINO
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

#else
	if(status != WL_CONNECTED)
	{
		log(string("Attempting to connect to SSID: ") + SECRET_SSID);
		// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
		WiFi.begin(SECRET_SSID, SECRET_PASS);
	}
	else
	{
		WiFiClient client = server.available();
		if(client)
		{
			WebRequest request(this, client);
			request.processRequest();

			delay(1);
			client.stop();
		}
	}
#endif
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

		log("\t" + name + "='" + value + "'");
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
	log("<<");
	log(request);
	log("<</");
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
#ifndef ARDUINO
	send(client, header.c_str(), header.length(), 0);
#else
	client.print(header.c_str());
#endif
}
