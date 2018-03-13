#include "LogAction.h"

std::string LogAction::GetStringFromType(LogAction::Type type)
{
	switch (type)
	{
		case LogAction::Type::Injured_Moved:
			return Name::CorrectName("Förflyttning av skadad personal");
		case LogAction::Type::Injured_Treated:
			return Name::CorrectName("Omhändertagen personal");
		case LogAction::Type::Injured_Reported:
			return Name::CorrectName("Inrapportering av skadad personal");
		case LogAction::Type::Hole_In_Bulk:
			return Name::CorrectName("Hål i skott");
		case LogAction::Type::Ventilation_In:
			return Name::CorrectName("Rökgasventilation in");
		case LogAction::Type::Ventilation_Out:
			return Name::CorrectName("Rökgasventliation ut");
		case LogAction::Type::Cooling_Wall:
			return Name::CorrectName("Kylning av brandgräns");
		case LogAction::Type::Supporting_Wall:
			return Name::CorrectName("Stöttning");
		case LogAction::Type::Damaged_Bulk:
			return Name::CorrectName("Skadat skott");
		case LogAction::Type::Draining:
			return Name::CorrectName("Länsning");
		case LogAction::Type::Seal_Hole:
			return Name::CorrectName("Tätning");
		default:
			return Name::CorrectName("Okänd åtgärd");
	}
}

LogAction::Type LogAction::GetTypeFromString(std::string type)
{
	if (type == Name::CorrectName("Förflyttning av skadad personal"))
		return LogAction::Type::Injured_Moved;
	if (type == Name::CorrectName("Omhändertagen personal"))
		return LogAction::Type::Injured_Treated;
	if (type == Name::CorrectName("Inrapportering av skadad personal"))
		return LogAction::Type::Injured_Reported;
	if (type == Name::CorrectName("Hål i skott"))
		return LogAction::Type::Hole_In_Bulk;
	if (type == Name::CorrectName("Rökgasventilation in"))
		return LogAction::Type::Ventilation_In;
	if (type == Name::CorrectName("Rökgasventliation ut"))
		return LogAction::Type::Ventilation_Out;
	if (type == Name::CorrectName("Kylning av brandgräns"))
		return LogAction::Type::Cooling_Wall;
	if (type == Name::CorrectName("Stöttning"))
		return LogAction::Type::Supporting_Wall;
	if (type == Name::CorrectName("Skadat skott"))
		return LogAction::Type::Damaged_Bulk;
	if (type == Name::CorrectName("Länsning"))
		return LogAction::Type::Draining;
	if (type == Name::CorrectName("Tätning"))
		LogAction::Type::Seal_Hole;

	// Default:
	return LogAction::Type::Seal_Hole;
}

LogAction::Type LogAction::GetTypeFromNumber(int number)
{
	switch (number)
	{
		case 0:
			return LogAction::Type::Injured_Moved;
		case 1:
			return LogAction::Type::Injured_Treated;
		case 2:
			return LogAction::Type::Injured_Reported;
		case 3:
			return LogAction::Type::Hole_In_Bulk;
		case 4:
			return LogAction::Type::Ventilation_In;
		case 5:
			return LogAction::Type::Ventilation_Out;
		case 6:
			return LogAction::Type::Cooling_Wall;
		case 7:
			return LogAction::Type::Supporting_Wall;
		case 8:
			return LogAction::Type::Damaged_Bulk;
		case 9:
			return LogAction::Type::Draining;
		case 10:
			return LogAction::Type::Seal_Hole;
		default:
			return LogAction::Type::Seal_Hole;
	}
}

LogAction::LogAction()
{
	this->mActive = true;
}

LogAction::LogAction(LogAction::Desc desc)
{
	this->mStart = desc.start;
	this->mActive = desc.active;
	this->mID = desc.ID;
	this->mRotation = desc.rotation;

	this->mType = desc.type;
	this->mRoomName = desc.roomName;
	this->mpActionIndex = desc.pActionIndex;

	this->mCoord.x = desc.pos_x;
	this->mCoord.z = desc.pos_z;

	if (LogAction::CheckHasNumber(this->mType))
	{
		this->mHasNumber = true;
		this->mIconNumber = desc.numberOnIcon;
	}
	else
		this->mHasNumber = false;
}

LogAction::LogAction(std::string lineFromLog, std::string metaLine)
{
	this->mActive = false; // Room log will set this active if it is

	std::stringstream ss(lineFromLog);
	std::string word;
	//int number;
	char scrap;

	Timestamp ts;

	ss.get(scrap); // Get rid of 'a'

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


	// Get Action type
	std::string actionType = "";
	ss >> word;
	while (word != "|")
	{
		if (actionType != "")
			actionType += " ";
		actionType += word;
		ss >> word;
	} // Exits with word == "|"
	this->mType = LogAction::GetTypeFromString(actionType);

	if (LogAction::CheckHasNumber(this->mType))
		this->mHasNumber = true;


	// Get active info
	ss >> word;
	if (word ==	Name::CorrectName("Påbörjad"))
		this->mStart = true;
	else // "Avslutad"
		this->mStart = false;

	ss >> word; // Get '|'


	// Get room name
	this->mRoomName = "";
	while (!ss.eof())
	{
		ss >> word;
		if (this->mRoomName != "")
			this->mRoomName += " ";
		this->mRoomName += word;
	}



	/**
	*	Fill information from the metafile
	*/

	ss.clear();
	ss.str(metaLine);

	ss >> this->mID;
	ss >> this->mRotation;
	ss >> this->mCoord.x;
	ss >> this->mCoord.z;

	if (this->mHasNumber)
	{
		ss >> this->mIconNumber;
	}
	else
		this->mIconNumber = -1;
}

LogAction::~LogAction()
{
	
}



/**
*	Action specific
*/

void LogAction::SetInactive()
{
	this->mActive = false;
}

void LogAction::SetActive()
{
	this->mActive = true;
}

void LogAction::SetType(LogAction::Type type)
{
	this->mType = type;
}

bool LogAction::IsActive() const
{
	return this->mActive;
}

LogAction::Type LogAction::GetType() const
{
	return this->mType;
}

int* LogAction::GetActionIndex()
{
	return this->mpActionIndex;
}

void LogAction::SetPos(float x, float z)
{
	this->mCoord.x = x;
	this->mCoord.z = z;
}

float LogAction::GetPos_X() const
{
	return this->mCoord.x;
}

float LogAction::GetPos_Z() const
{
	return this->mCoord.z;
}

int LogAction::GetID() const
{
	return this->mID;
}

uint32_t LogAction::GetRotation() const
{
	return this->mRotation;
}



/**
*	Log specific
*/

std::string LogAction::GetLogString() const
{
	/**
	*	Will return a one-line string to be printed to the .log file. Ex:
	*	Jan 23 12:17:03		Kylning av brandgräns	|	Påbörjad	|	Maskinrum
	*/

	std::stringstream ss;
	Timestamp ts;

	// Fill timestamp from timer
	ts = this->mTimer.GetTimestamp();

	// Pass date to string
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



	// Pass time to string
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

	std::string typeString = LogAction::GetStringFromType(this->mType);

	// Pass action type to string
	ss << "\t\t" << typeString;

	ss << Name::GetTabs(typeString.size());

	// Pass active information to string
	if (this->mStart)
		ss << "|\t" << Name::CorrectName("Påbörjad") << "\t|\t";
	else
		ss << "|\tAvslutad\t|\t";



	// Pass room name to string
	ss << this->mRoomName;

	return ss.str();
}

std::string LogAction::GetMetaString() const
{
	std::stringstream ss;

	ss	<< this->mID
		<< " "
		<< this->mRotation
		<< " "
		<< this->mCoord.x
		<< " "
		<< this->mCoord.z;

	if (this->mHasNumber)
	{
		ss << " " << this->mIconNumber;
	}

	return ss.str();
}



/**
*	Room specific
*/

void LogAction::SetRoomName(std::string name)
{
	this->mRoomName = name;
}

std::string LogAction::GetRoomName() const
{
	return this->mRoomName;
}



/**
*	Time specific
*/

const int LogAction::GetElapsedTime()
{
	return this->mTimer.GetSeconds();
}

const std::string LogAction::GetStartTimeAsString()
{
	return this->mTimer.WhenTimerStarted();
}

const std::string LogAction::GetElapsedTimeAsString()
{
	return this->mTimer.GetTimeAsStr();
}

Timestamp LogAction::GetTimestamp_Start() const
{
	return this->mTimer.GetTimestamp();
}

int LogAction::GetSecondsSinceStart() const
{
	return this->mTimer.GetSecondsSinceStart();
}

bool LogAction::CheckHasNumber(LogAction::Type type)
{
	if (type == LogAction::Type::Injured_Moved ||
		type == LogAction::Type::Injured_Treated ||
		type == LogAction::Type::Injured_Reported)
		return true;

	return false;
}
