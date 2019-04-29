#include "Helper.h"
#include "WebServer.h"
#include "defines.h"


WebServer::WebServer()//:_server(80)
{
	Serial.println("WebServer - Start UP !");
	//Serial.println("Creating access point named: ");
	//Serial.println(SECRET_SSID);

	/*_WiFiState = WiFi.beginAP(SECRET_SSID);
	if(_WiFiState != WL_AP_LISTENING)
	{
		Serial.println("Creating access point failed");
		while(true);
	}*/
}