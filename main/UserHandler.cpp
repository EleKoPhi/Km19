#include "FileIO.h"
#include "stdafx.h"
#include "Helper.h"
#include "UserHandler.h"
#include "defines.h"


#ifndef ARDUINO
#include <ctime> 
#include <filesystem>
#else
#include <SPI.h>
#include <MFRC522.h>
#endif

const string UserHandler::UnknownUser = "unkonwn";

UserHandler::UserHandler()
{
	SPI.begin();
	realTimeClock();
	//_nfcReader.PCD_Init();
	sdCard();
	nfcReader();
	pinMode(taster_LINKS_pin, INPUT);
	pinMode(taster_RECHTS_pin, INPUT);
}

UserHandler::~UserHandler()
{
}

UserHandler* UserHandler::getInstance()
{
	static UserHandler instance;
	return &instance;
}
#ifndef ARDUINO
TimeStamp TimeStamp::now()
{
	std::time_t t = std::time(0);   // get time now
	std::tm* now = std::localtime(&t);
	return TimeStamp(now);
}

TimeStamp::TimeStamp(tm * timestamp)
	: year(timestamp->tm_year + 1900)
	, month(timestamp->tm_mon + 1)
	, day(timestamp->tm_mday)
	, hours(timestamp->tm_hour)
	, minutes(timestamp->tm_min)
	, seconds(timestamp->tm_sec)

{
}
#else
TimeStamp TimeStamp::now()
{
	auto rtc = UserHandler::realTimeClock();
	rtc->begin();
	DateTime now = rtc->now();
	UserHandler::nfcReader();  // Shit :O
	return TimeStamp(now);
}

TimeStamp::TimeStamp(DateTime& timestamp)
	: year(timestamp.year())
	, month(timestamp.month())
	, day(timestamp.day())
	, hours(timestamp.hour())
	, minutes(timestamp.minute())
	, seconds(timestamp.second())
{
}

#endif

string padLeft(string text, int length, char padding)
{
	while(text.length() < length) text = padding + text;
	return text;
}

string padLeft(int value, int length, char padding)
{
	string text = to_string(value);
	return padLeft(text, length, padding);
}

TimeStamp TimeStamp::parse(string timestamp)
{
	string year, month, day, hours, minutes, seconds;
	year = timestamp.substr(0, 4);
	month = timestamp.substr(5, 2);
	day = timestamp.substr(8, 2);
	hours = timestamp.substr(11, 2);
	minutes = timestamp.substr(14, 2);
	seconds = timestamp.substr(17, 2);
	return TimeStamp(stoi(year), stoi(month), stoi(day), stoi(hours), stoi(minutes), stoi(seconds));
}

TimeStamp::TimeStamp(int year, int month, int day, int hours, int minutes, int seconds)
	: year(year)
	, month(month)
	, day(day)
	, hours(hours)
	, minutes(minutes)
	, seconds(seconds)
{
}

string TimeStamp::getTimestamp()
{
	return padLeft(year, 4) + "-" + padLeft(month) + "-" + padLeft(day) + "_" + padLeft(hours) + "-" + padLeft(minutes) + "-" + padLeft(seconds);
}

string TimeStamp::getDate()
{
	return padLeft(year, 4) + "-" + padLeft(month) + "-" + padLeft(day);
}

string TimeStamp::getTime()
{
	return padLeft(hours) + ":" + padLeft(minutes) + ":" + padLeft(seconds);
}

string TimeStamp::getDateTime()
{
	return getDate() + " " + getTime();
}

bool startswith(const string& text, string token)
{
	token = token + ";";
	size_t length = token.length();
	for(size_t i = 0; i < length; i++)
	{
		if(text[i] != token[i])
			return false;
	}
	return true;
}



std::map<string, User> UserHandler::readUsers()
{
#ifdef ARDUINO
	sdCard();
#endif
	FileReader reader(UserFile);

	std::map<string, User> users;
	string line;
	while(reader.readLine(line))
	{
		User user;
		int firstsep = line.find(';');
		int lastsep = line.find_last_of(';');

		if(lastsep == firstsep)
		{
			user.isAllowed = "1";
			lastsep = line.length();
		}
		else
		{
			user.isAllowed = line.substr(lastsep + 1) == "1";
		}
		++firstsep;
		user.cardId = line.substr(0, firstsep - 1);
		user.name = line.substr(firstsep, lastsep - firstsep);
		users[user.cardId] = user;
		log("read user: " + user.toString());
	}

	return users;
}

string UserHandler::readCurrentCardId()
{
	long code = 0;
	auto nfc = nfcReader();
	if(nfc->PICC_ReadCardSerial())
	{
		for(byte i = 0; i < nfc->uid.size; i++)
		{
			code = ((code + nfc->uid.uidByte[i]) * 10);
		}
	}
	return to_string(code);
}

string UserHandler::checkUser(const string & cardId)
{
#ifdef ARDUINO
	sdCard();
#endif
	FileReader reader(UserFile);
	string line;
	while(reader.readLine(line))
	{
		if(startswith(line, cardId))
		{
			string username;
			string isallowed;
			int firstsep = line.find(';');
			int lastsep = line.find_last_of(';');

			if(lastsep == firstsep)
			{
				isallowed = "1";
				lastsep = line.length();
			}
			else
			{
				isallowed = line.substr(lastsep + 1);
			}
			++firstsep;
			username = line.substr(firstsep, lastsep - firstsep);

			if(isallowed == "1")
				return username;
			else
				return UserHandler::UnknownUser;
		}
	}
	return UserHandler::UnknownUser;
}

void UserHandler::setUser(const string & cardIdInput, const string & name, bool isAllowed)
{
#ifdef ARDUINO
	sdCard();
	renameFile(UserFile, UserFile + ".bak");
#else
	string folder = TestFolder;
	experimental::filesystem::rename(folder + UserFile, folder + UserFile + ".bak");
#endif
	FileReader reader(UserFile + ".bak");
	FileWriter writer(UserFile);

	string cardId = LogEntry::stdCardId(cardIdInput);

	string line;
	bool replaced = false;
	while(reader.readLine(line))
	{

		if(startswith(line, cardId))
		{
			line = cardId + ";" + name + ";" + (isAllowed ? "1" : "0");
			replaced = true;
		}
		writer.write(line + "\n");
	}
	if(!replaced)
	{
		line = cardId + ";" + name + ";" + (isAllowed ? "1" : "0");
		writer.write(line + "\n");
	}
}

void UserHandler::getUserStatistics(int & number, int & numberBlocked, int & numberUnnamed)
{
#ifdef ARDUINO
	sdCard();
#endif
	number = 0;
	numberBlocked = 0;
	numberUnnamed = 0;
	FileReader reader(UserFile);
	string line;
	while(reader.readLine(line))
	{
		if(line.length() < 4)
			continue;
		string username;
		string isallowed;
		int firstsep = line.find(';');
		int lastsep = line.find_last_of(';');

		if(lastsep == firstsep)
		{
			isallowed = "1";
			lastsep = line.length();
		}
		else
		{
			isallowed = line.substr(lastsep + 1);
		}
		++firstsep;
		username = line.substr(firstsep, lastsep - firstsep);
		if(username == "")
			numberUnnamed++;
		if(isallowed != "1")
			numberBlocked++;
		number++;
	}
}

string UserHandler::writeLogLine(TimeStamp & timestamp, const string & cardId, LogEntryType type)
{
	LogEntry entry(timestamp, cardId, type);
	string line = entry.toCsv();
	return line;
}

void UserHandler::writeLog(const string & cardId, LogEntryType entry)
{
	FileWriter writer(LogFile);

	auto now = TimeStamp::now();
	string line = writeLogLine(now, cardId, entry);
	writer.write(line);
}

vector<LogEntry> UserHandler::readLog(unsigned int maximum, string filename)
{
	if(filename == "") filename = LogFile;
	FileReader reader(filename);
	streamoff maxlen = (logEntryLength * maximum);
	streamoff length = reader.length();
	if(length < maxlen)
		reader.seekStart();
	else
		reader.seek(length - maxlen);
	string line;
	vector<LogEntry> entries;
	for(int lines = 0; lines < maximum && reader.readLine(line); lines++)
	{
		entries.push_back(LogEntry());
		LogEntry* entry = &entries[lines];

		string cardid;
		string type;
		string timestamp;
		int firstsep = line.find(';');
		int lastsep = line.find_last_of(';');

		if(lastsep == firstsep)
		{
			type = "1";
			lastsep = line.length();
		}
		else
		{
			type = line.substr(lastsep + 1);
			if(type[type.length() - 1] == '\r')
				type.replace(type.length() - 1, 1, "");
		}
		++firstsep;
		cardid = line.substr(firstsep, lastsep - firstsep);
		timestamp = line.substr(0, firstsep);

		entry->timestamp = TimeStamp::parse(timestamp);
		entry->cardId = cardid;
		entry->type = (LogEntryType)stoi(type);
	}
	return entries;
}

int UserHandler::numberOfLogEntries()
{
	FileReader reader(LogFile);
	streamoff length = reader.length();
	int entries = length / logEntryLength;
	return entries;
}

#ifndef ARDUINO
namespace fs = std::experimental::filesystem;
vector<string> UserHandler::getOldLogFiles()
{
	vector<string> files;
	for(auto& p : fs::directory_iterator(TestFolder))
	{
		auto path = p.path();
		auto filename = path.filename();
		auto text = filename.string();
		if(text.find("_log.csv") != string::npos)
			files.push_back(text);
	}
	return files;
}
#else
vector<string> UserHandler::getOldLogFiles()
{
	vector<string> files;
	auto dir = FileIOBase::SD.open("/");
	while(true)
	{
		File entry = dir.openNextFile();
		if(!entry)
			// no more files
			break;

		if(!entry.isDirectory())
		{
			string text = entry.name();
			if(text.find("_log.csv") != string::npos)
				files.push_back(text);
		}
		entry.close();
	}
	return files;
}
#endif

void UserHandler::setParameter(const string & name, const double value)
{
	setParameter(name, to_string(value));
}

void UserHandler::setParameter(const string & name, const string & value = "")
{
#ifdef ARDUINO
	sdCard();
	renameFile(ConfigFile, ConfigFile + ".bak");
#else
	string folder = TestFolder;
	experimental::filesystem::rename(folder + ConfigFile, folder + ConfigFile + ".bak");
#endif
	FileReader reader(ConfigFile + ".bak");
	FileWriter writer(ConfigFile);

	string line;
	bool replaced = false;
	while(reader.readLine(line))
	{

		if(startswith(line, name))
		{
			line = name + ";" + value;
			replaced = true;
		}
		writer.write(line + "\n");
	}
	if(!replaced)
	{
		line = name + ";" + value;
		writer.write(line + "\n");

	}
}

string UserHandler::getParameter(const string & name)
{
	FileReader reader(ConfigFile);
	string line;
	while(reader.readLine(line))
	{
		if(startswith(line, name))
		{
			int seperator = line.find(';');
			string value = line.substr(seperator + 1);
			if(value[value.length() - 1] == '\r')
				value.replace(value.length() - 1, 1, "");
			return value;
		}
	}
	return "";
}

double UserHandler::getParameterD(const string & name)
{
	return stod(getParameter(name));
}

vector<Parameter> UserHandler::getParameters()
{
	vector<Parameter> params;
	FileReader reader(ConfigFile);
	string line;
	while(reader.readLine(line))
	{
		int seperator = line.find(';');
		string value = line.substr(seperator + 1);
		if(value[value.length() - 1] == '\r')
			value.replace(value.length() - 1, 1, "");
		string param = line.substr(0, seperator);
		params.push_back(Parameter(param, value));
	}
	return params;
}

MillStates UserHandler::readButtonChoice()
{
	if(digitalRead(taster_LINKS_pin) /*&& !deboundeStatus*/)
	{
		//deboundeStatus = true;
		Serial.println("l");
		return MillStates::Einfach;
	}
	if(digitalRead(taster_RECHTS_pin)/* && !deboundeStatus*/)
	{
		//deboundeStatus = true;
		Serial.println("r");
		return MillStates::Doppelt;
	}

	return MillStates::WaitForInput;
}

bool UserHandler::isCardAvailable()
{
	auto nfc = nfcReader();
	return nfc->PICC_IsNewCardPresent() == true;

}

string LogEntry::stdCardId(const string & cardId)
{
	return padLeft(cardId, 20);
}

string LogEntry::csvHeader()
{
	return "timestamp;cardId;type[0=none,1=single,2=double];name\n";
}

string LogEntry::toCsv(string username)
{
	return timestamp.getTimestamp() + ";" + stdCardId(cardId) + ";" + to_string(type) + (username != "" ? ";" + username : "") + "\n";
}
