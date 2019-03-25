#include "Arduino.h"
#include "UserHandler.h"
#include "defines.h"
#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>


UserHandler::UserHandler(int chipSelect, int slaveSelect, int rstPin) : _nfcReader(slaveSelect, rstPin)
{
  _cspin = chipSelect;
  SdStatus = false;
}

String UserHandler::CheckIfExists(String cardID)
{
  SD.begin(_cspin);
  _userData = SD.open("USERTX~1.TXT", FILE_READ);

  char cardBuf[cardID.length() + 1];
  char UserFiled[_userData.available()];
  int idIterator = 0;
  int filedIterator = 0;


  cardID.toCharArray(cardBuf, sizeof(cardBuf));
  _userData.read(UserFiled, sizeof(UserFiled));

  bool compareFlag = true;

  for (filedIterator = 0; filedIterator <= sizeof(UserFiled) - 1; filedIterator++)
  {

    if (UserFiled[filedIterator] == ':') compareFlag = false;

    if (UserFiled[filedIterator] == '\n')
    {
      compareFlag = true;
      idIterator = 0;
      continue;
    }

    if (compareFlag)
    {

      if (UserFiled[filedIterator] != cardBuf[idIterator])
      {

        compareFlag = false;
        continue;
      }

      if (idIterator + 1 == sizeof(cardBuf) - 1 & UserFiled[filedIterator + 1] == ':')
      {

        int i = 0;
        String userName;
        while (UserFiled[filedIterator + i + 1] != '\n')
        {
          userName += String(UserFiled[filedIterator + i + 2]);
          i++;
        }
        _userData.close();
        return userName;
      }
    }
    idIterator++;
  }

  _userData.close();
  return String("Unknow user");
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
  if (digitalRead(taster_LINKS_pin))return 'l';
  if (digitalRead(taster_RECHTS_pin))return 'r';
  return 'n';
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

  if (_nfcReader.PICC_ReadCardSerial() )
  {
    for (byte i = 0; i < _nfcReader.uid.size; i++)
    {
      code = ((code + _nfcReader.uid.uidByte[i]) * 10);
    }
  }
  Serial.println(GetMoment());
  return String(code, DEC);
}

String UserHandler::GetMoment()
{
  String Moment = "";
  _rtc.begin();
  DateTime now = _rtc.now();
  _nfcReader.PCD_Init();  // Shit :O
  Moment = String(now.year(), DEC) + "-" + String(now.month(), DEC) + "-" + String(now.day(), DEC) + " " + String(now.hour(), DEC) + ":" + String(now.minute(), DEC) + ":" + String(now.second(), DEC);
  return Moment;
}

void UserHandler::WriteToLog(char state, String user1, String user2)
{

  Serial.println(user1);
  Serial.println(user2);
  this->_logFile = SD.open("UserLog.txt", FILE_WRITE);

  if (this->_logFile)
  {
    if (state == Einfach)
    {
      String logLine = "1_" + user1 + "_" + GetMoment();
      Serial.println(logLine);
      this->_logFile.println(logLine);
    }

    if (state == Doppelt)
    {
      if (user2.c_str() == "")
      {
        String logLine = "2_" + user1 + "_" + GetMoment();
        Serial.println(logLine);
        this->_logFile.println(logLine);
      }
      else
      {
        String logLine1 = "1_" + user1 + "_" + GetMoment();
        Serial.println(logLine1);
        String logLine2 = "1_" + user2 + "_" + GetMoment();
        Serial.println(logLine2);
        this->_logFile.println(logLine1);
        this->_logFile.println(logLine2);
      }
    }
  }
  else
  {
    Serial.println("Cant write to card");
    while (true) {}
  }

  this->_logFile.close();

}
