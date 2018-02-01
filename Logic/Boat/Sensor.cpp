#include "Sensor.h"

Sensor::Sensor()
{
	this->mActiveEventIndex = -1;
}

Sensor::~Sensor()
{
}



/**
*	Sensor specific
*/

void Sensor::AddInputType(Event::Type type)
{
	// Check early exit
	for (int i = 0; i < (int)this->mInputTypes.size(); i++)
	{
		// If already exists
		if (mInputTypes[i] == type)
			return;
	}
	
	this->mInputTypes.push_back(type);
}

void Sensor::RemoveInputType(Event::Type type)
{
	// Find type in list
	for (int i = 0; i < (int)this->mInputTypes.size(); i++)
	{
		// If found, remove it
		if (mInputTypes[i] == type)
			this->mInputTypes.erase(this->mInputTypes.begin() + i);
	}
}

bool Sensor::CanDetect(Event::Type type) const
{
	for (int i = 0; i < (int)this->mInputTypes.size(); i++)
	{
		if (mInputTypes[i] == type)
			return true;
	}

	// Type not found in list
	return false;
}

int Sensor::AutoTrigger(Event::Type type)
{
	// Check early exit
	if (!this->CanDetect(type))
		return -1;

	this->mActiveEventIndex = this->mpEventLog->AddEvent(type, this->mRoomIndex);
	return this->mActiveEventIndex;
}

int Sensor::PlotTrigger(Event::Type type)
{
	this->mActiveEventIndex = this->mpEventLog->AddEvent(type, this->mRoomIndex);
	return this->mActiveEventIndex;
}



/**
*	Log specific
*/

void Sensor::SetEventLog(EventLog *pEventLog)
{
	this->mpEventLog = pEventLog;
}



/**
*	Event specific
*/

void Sensor::SetActiveEventIndex(int index)
{
	this->mActiveEventIndex = index;
}



/**
*	Room specific
*/

void Sensor::SetRoomIndex(int index)
{
	this->mRoomIndex = index;
}



/**
*	Disk specific
*/

std::string Sensor::WriteString() const
{
	std::stringstream ss;
	
	ss << "sensor ";
	ss << this->mActiveEventIndex;
	ss << " { ";

	for (int i = 0; i < (int)this->mInputTypes.size(); i++)
		ss << this->mInputTypes[i] << " ";
		//print += Event::GetID(this->mInputTypes[i]) + " ";

	ss << "}";
	
	return ss.str();
}
