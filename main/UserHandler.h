#ifndef UserHandler_h
#define UserHandler_h

#include "Arduino.h"
#include <SD.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <RTClib.h>

using string = String;

class UserHandler
{
  public:
    UserHandler(int chipSelect, int slaveSelect, int rstPin);
    
	string CheckIfExists(string cardID);
    void StartUp();
    char ReadUserInput();
    bool HasCardToRead();
	string GetCardId();
	string getTimestamp();
    void WriteToLog(char state , string user1, string user2, string userId1, string userId2);
	string GetLastUser();

    bool SdStatus;
    bool NfcStatus;
    bool RtcStatus;

	const string UnknownUser = "unkonwn";

  private:
    MFRC522 _nfcReader;
    File _userData;
    File _logFile;
    RTC_DS3231 _rtc;
    int _cspin;

	bool deboundeStatus;

};

#endif
