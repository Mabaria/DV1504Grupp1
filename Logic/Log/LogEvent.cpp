#include "LogEvent.h"

LogEvent::LogEvent()
{
	this->mActive = true;
}

LogEvent::LogEvent(LogEvent::Desc desc)
{

	this->mType = desc.type;
	this->mRoomName = desc.roomName;
	this->mStartEvent = desc.start;
	this->mActive = desc.active;
	this->mID = desc.ID;
}

LogEvent::LogEvent(std::string lineFromLog, int ID)
{
	this->mActive = false; // Room log will set this active if it is

	std::stringstream ss(lineFromLog);
	std::string word;
	std::string number;
	char scrap;

	Timestamp ts;

	ss >> word; // Get rid of 'e'

	// Fill the start time to timestamp
	ss >> ts.date.year;
	ss.get(scrap);
	ss >> ts.date.month;
	ss.get(scrap);
	ss >> ts.date.day;
	
	ss >> ts.clock.hour;
	ss.get(scrap);
	ss >> ts.clock.minute;
	ss.get(scrap);
	ss >> ts.clock.second;

	this->mTimer.SetTimestamp(ts);

	// Get event type
	ss >> word;
	this->mType = Event::GetType(word);

	ss >> word; // '|'

	// Get active info
	ss >> word;
	if (word ==	Name::CorrectName("Påbörjad"))
		this->mStartEvent = true;
	else // "Avslutad"
		this->mStartEvent = false;

	ss >> word; // '|'

	// Get name of room
	this->mRoomName = "";
	while (!ss.eof())
	{
		ss >> word;
		if (this->mRoomName != "")
			this->mRoomName += " ";
		this->mRoomName += word;
	}

	this->mID = ID;
}

LogEvent::~LogEvent()
{

}

/**
*	Event specific
*/

void LogEvent::SetType(Event::Type type)
{
	this->mType = type;
}

Event::Type LogEvent::GetType() const
{
	return this->mType;
}

bool LogEvent::IsActive() const
{
	return this->mActive;
}

bool LogEvent::IsStartEvent() const
{
	return this->mStartEvent;
}

void LogEvent::SetInactive()
{
	this->mActive = false;
}

void LogEvent::SetActive()
{
	this->mActive = true;
}

int LogEvent::GetID() const
{
	return this->mID;
}



/**
*	Log specific
*/

std::string LogEvent::GetFileString()
{
	/**
	*	Will return a one-line string to be printed to the .log file. Ex:
	*	Jan 23 12:17:03		Eldsvåda	|	startad		|	Maskinrum
	*/

	std::stringstream ss;
	Timestamp ts;

	ts = this->mTimer.GetTimestamp();

	if (ts.date.year < 10)
		ss << 0 << ts.date.year << "-";
	else
		ss << ts.date.year << "-";

	if (ts.date.month < 10)
		ss << 0 << ts.date.month << "-";
	else
		ss << ts.date.month << "-";

	if (ts.date.day < 10)
		ss << 0 << ts.date.day << " ";
	else
		ss << ts.date.day << " ";



	if (ts.clock.hour < 10)
		ss << 0 << ts.clock.hour << ":";
	else
		ss << ts.clock.hour << ":";

	if (ts.clock.minute < 10)
		ss << 0 << ts.clock.minute << ":";
	else
		ss << ts.clock.minute << ":";

	if (ts.clock.second < 10)
		ss << 0 << ts.clock.second;
	else
		ss << ts.clock.second;


	std::string type = Event::GetString(this->mType);
	ss << "\t\t" << type;

	ss << Name::GetTabs(type.size());

	if (this->mStartEvent)
		ss << "|\t" << Name::CorrectName("Påbörjad") << "\t|\t" << this->mRoomName;
	else
		ss << "|\tAvslutad\t|\t" << this->mRoomName;

	return ss.str();
}

/**
*	Room specific
*/

void LogEvent::SetRoomName(std::string name)
{
	this->mRoomName = name;
}

std::string LogEvent::GetRoomName() const
{
	return this->mRoomName;
}



/**
*	Time specific
*/

void LogEvent::StartTimer()
{
	this->mTimer.StartTimer();
}

const int LogEvent::GetElapsedTime()
{
	return this->mTimer.GetSeconds();
}

const std::string LogEvent::GetStartTimeAsString()
{
	return this->mTimer.WhenTimerStarted();
}

const std::string LogEvent::GetElapsedTimeAsString()
{
	return this->mTimer.GetTimeAsStr();
}

Timestamp LogEvent::GetTimestamp_Start() const
{
	return this->mTimer.GetTimestamp();
}

Timer * LogEvent::GetTimer()
{
	return &this->mTimer;
}

int LogEvent::GetSecondsSinceStart() const
{
	return this->mTimer.GetSecondsSinceStart();
}

bool LogEvent::operator < (const LogEvent &other)
{
	if (this->mTimer.GetSecondsSinceStart() < other.GetSecondsSinceStart())
		return true;
	return false;
}




