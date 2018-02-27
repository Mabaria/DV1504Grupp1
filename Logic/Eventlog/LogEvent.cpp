#include "LogEvent.h"

LogEvent::LogEvent()
{

}

LogEvent::LogEvent(Event::Type type, int roomEventIndex)
{
	this->mType = type;
	this->mActiveEventIndex = roomEventIndex;
}

LogEvent::LogEvent(std::string lineFromLog)
{
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

	// Get started/stopped
	ss >> word;
	// TODO: Make this useful

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
	

	//ss >> this->mRoomName;
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



/**
*	Log specific
*/

void LogEvent::SetActiveEventIndex(int index)
{
	this->mActiveEventIndex = index;
}

int LogEvent::GetActiveEventIndex() const
{
	return this->mActiveEventIndex;
}

std::string LogEvent::GetFileString()
{
	/**
	*	Will return a one-line string to be printed to the .log file. Ex:
	*	Jan 23 12:17:03 Eldsvåda startade i Maskinrum
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

	if (type.size() < 4)
		ss << "\t\t\t\t|\t";
	else if (type.size() < 8)
		ss << "\t\t\t|\t";
	else if (type.size() < 12)
		ss << "\t\t|\t";
	else
		ss << "\t|\t";

	ss << "startad\t\t|\t" << this->mRoomName;

	return ss.str();
}

/**
*	Room specific
*/

void LogEvent::SetRoomName(std::string name)
{
	this->mRoomName = name;
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

Timer * LogEvent::GetTimer()
{
	return &this->mTimer;
}




