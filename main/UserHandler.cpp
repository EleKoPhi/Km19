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
  deboundeStatus = false;
}

String UserHandler::CheckIfExists(String cardID)
{
  SD.begin(_cspin);
  _userData = SD.open("user.txt", FILE_READ);

  char cardBuf[cardID.length() + 1];
  char UserFiled[_userData.available()];
  int idIterator = 0;
  int filedIterator = 0;


  cardID.toCharArray(cardBuf, sizeof(cardBuf));
  _userData.read(UserFiled, sizeof(UserFiled));

  _userData.close();

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
		  if(String(UserFiled[filedIterator + i + 2]) == "\n") break;
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
	/*if(!digitalRead(taster_LINKS_pin) && !digitalRead(taster_RECHTS_pin))
	{
		deboundeStatus = false;
	}*/

	if(digitalRead(taster_LINKS_pin) /*&& !deboundeStatus*/)
	{
		//deboundeStatus = true;
		Serial.println("l");
		return 'l';
	}
	if(digitalRead(taster_RECHTS_pin)/* && !deboundeStatus*/)
	{
		//deboundeStatus = true;
		Serial.println("r");
		return 'r';
	}

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
  return String(code, DEC);
}

String UserHandler::getTimestamp()
{
  String Moment = "";
  _rtc.begin();
  DateTime now = _rtc.now();
  _nfcReader.PCD_Init();  // Shit :O
  Moment = String(now.year(), DEC) + "-";
  if(now.month() < 10)Moment += "0";
  Moment += String(now.month(), DEC) + "-";
  if(now.day() < 10)Moment += "0";
  Moment += String(now.day(), DEC) + " ";
  if(now.hour() < 10)Moment += "0";
  Moment += String(now.hour(), DEC) + ":";
  if(now.minute() < 10)Moment += "0";
  Moment += String(now.minute(), DEC) + " ";

  return Moment;
}

void UserHandler::WriteToLog(char state, String user1, String user2, String userId1, String userId2)
{

  this->_logFile = SD.open("UserLog.txt", FILE_WRITE);

  if (this->_logFile)
  {
    if (state == Einfach)
    {
	  String logLine = getTimestamp() + ";" + user1 + ";" + userId1 + ";1";
      this->_logFile.println(logLine);
    }

    if (state == Doppelt)
    {
      if (user2.c_str() == "")
      {
		String logLine = getTimestamp() + ";" + user1 + ";" + userId1 + ";2";
        this->_logFile.println(logLine);
      }
      else
      {
		String logLine1 = getTimestamp() + ";" + user1 + ";" + userId1 + ";1";
		String logLine2 = getTimestamp() + ";" + user2 + ";" + userId2 + ";1";
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

String UserHandler::GetLastUser()
{
	SD.begin(_cspin);
	_logFile = SD.open("userLog.txt", FILE_READ);

	String log = "";

	while(_logFile.read() != -1)
	{
		log += String(_logFile.read());
	}

	_logFile.close();

	Serial.println(log);
}

