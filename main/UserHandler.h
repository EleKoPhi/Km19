#ifndef UserHandler_h
#define UserHandler_h

#include "Arduino.h"
#include <SD.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include "RTClib.h"

class UserHandler
{
  public:
    UserHandler(int chipSelect, int slaveSelect, int rstPin);
    
    String CheckIfExists(String cardID);
    void StartUp();
    char ReadUserInput();
    bool HasCardToRead();
    String GetCardId();
    String GetMoment();
    void WriteToLog(char state ,String user1, String user2);
    
    bool SdStatus;
    bool NfcStatus;
    bool RtcStatus;

    

  private:
    MFRC522 _nfcReader;
    File _userData;
    File _logFile;
    RTC_DS3231 _rtc;
    int _cspin;

};

#endif
