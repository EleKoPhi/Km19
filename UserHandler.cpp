#include "Arduino.h"
#include "UserHandler.h"
#include "defines.h"
#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>

byte PSWBuff[] = {0xFF, 0xAB, 0xBA, 0xFF};
byte pACK[] = {0xE, 0x5};


UserHandler::UserHandler(int chipSelect, int slaveSelect, int rstPin) : _nfcReader(slaveSelect, rstPin)
{
  _cspin = chipSelect;
  SdStatus = false;
  deboundeStatus = false;
}

String UserHandler::CheckIfExists(String cardID)
{

  return "Function not in use";
}

void UserHandler::StartUp()
{
  SPI.begin();
  RtcStatus = _rtc.begin();
  _nfcReader.PCD_Init();
  SdStatus = SD.begin(_cspin);
  _nfcReader.PCD_Init();
  NfcStatus = _nfcReader.PCD_PerformSelfTest();
  _nfcReader.PCD_Init();
  pinMode(taster_LINKS_pin, INPUT);
  pinMode(taster_RECHTS_pin, INPUT);
}


char UserHandler::ReadUserInput()
{
  if (digitalRead(taster_LINKS_pin))
  {
    while(digitalRead(taster_LINKS_pin)){}
    return 'l';
  }
  else if (digitalRead(taster_RECHTS_pin))
  {
    //while(digitalRead(taster_RECHTS_pin)){}
    return 'r';
  }
  else
  {
    return 'n';
  }
}

bool UserHandler::HasCardToRead()
{
  if (!_nfcReader.PICC_IsNewCardPresent())
  {
    return false;
  }
  else
  {
    return true;
  }
}

String UserHandler::GetCardId()
{
  long code = 0;

  if (_nfcReader.PICC_ReadCardSerial())
  {
    for (byte i = 0; i < _nfcReader.uid.size; i++)
    {
      code = ((code + _nfcReader.uid.uidByte[i]) * 10);
    }
  }

  Serial.println(String(code, DEC));
  return String(code, DEC);
}

String UserHandler::GetMoment()
{
  String Moment = "";
  _rtc.begin();
  DateTime now = _rtc.now();
  _nfcReader.PCD_Init();  // Shit :O
  Moment = String(now.year(), DEC) + "-";
  if (now.month() < 10)Moment += "0";
  Moment += String(now.month(), DEC) + "-";
  if (now.day() < 10)Moment += "0";
  Moment += String(now.day(), DEC) + " ";
  if (now.hour() < 10)Moment += "0";
  Moment += String(now.hour(), DEC) + ":";
  if (now.minute() < 10)Moment += "0";
  Moment += String(now.minute(), DEC) + " ";

  return Moment;
}

void UserHandler::WriteToLog(String userID, String credit, bool doppelt)
{

  this->_logFile = SD.open("UserLog.txt", FILE_WRITE);

  if (this->_logFile)
  {
    String logLine = "";
    if(doppelt)
    {
      logLine = GetMoment() + ";" + userID + ";" + credit + ";Doppelt";
    }
    else
    {
     logLine = GetMoment() + ";" + userID + ";" + credit + ";Einfach"; 
    }
    
    this->_logFile.println(logLine);
  }
  else
  {
    Serial.println("Cant write to card");
    while (true) {}
  }
  this->_logFile.close();
}

String UserHandler::GetConfig()
{
  SD.begin(_cspin);
  _logFile = SD.open("config.txt", FILE_READ);

  String logStream = "";

  String firstline = "";
  String secondline = "";
  
  bool flag = false;

  while (_logFile.available())
  {

    char input = _logFile.read();
    logStream+=input;
    
  }

  _logFile.close();

  firstline = logStream.substring(8,13);
  secondline = logStream.substring(23,28);

  return (firstline + "#" + secondline + "-");
}

int UserHandler::ReadCredit()
{
  byte buffer[18];
  byte byteCount;
  int stat = 0;
  byteCount = sizeof(buffer);

  stat = _nfcReader.MIFARE_Read(0x4, buffer, &byteCount);

  if (stat == 0) {
    return (buffer[0] + buffer[1] + buffer[2] + buffer[3]) / 4;
  }
  else
  {
    return -1;
  }
}

int UserHandler::WriteCredit(int newCredit,bool doppelt)
{

  byte PSWBuff[] = {0xFF, 0xAB, 0xBA, 0xFF};
  byte pACK[] = {0xE, 0x5};
  long userID = 0;
  int stat = 0;

  ReadCredit();
  _nfcReader.PCD_NTAG216_AUTH(&PSWBuff[0], pACK);

  for (byte i = 0; i < _nfcReader.uid.size; i++)
  {
    userID = ((userID + _nfcReader.uid.uidByte[i]) * 10);  
  }
  
  byte WBuff[] = {newCredit, newCredit, newCredit, newCredit};
  stat = _nfcReader.MIFARE_Ultralight_Write(0x4, WBuff, 4);

  if (stat != 0)
  {
    return -1;
  }

  ReadCredit();

  //userID = GetCardId();

  String UserTxT = String(userID,DEC);
 // Serial.println("UserTxT " + UserTxT);
  
  String CreditTxT = String(newCredit,DEC);
  //Serial.println("CreditTxT " + CreditTxT);
  
  this->WriteToLog(UserTxT.c_str(), CreditTxT.c_str(),doppelt);

  //Serial.println("End of write credit - returned 0");

  return 0;
}


String UserHandler::ID()
{
  long code = 0;

  _nfcReader.PICC_ReadCardSerial();
  Serial.println("UserHandler.ID()");

  Serial.println(_nfcReader.uid.size);

  for (byte i = 0; i < _nfcReader.uid.size; i++)
  {
    code = ((code + _nfcReader.uid.uidByte[i]) * 10);
  }

  Serial.println(String(code, DEC));
  return String(code, DEC);

}

void UserHandler::newRead()
{
  while (!_nfcReader.PICC_IsNewCardPresent()) {};
}
