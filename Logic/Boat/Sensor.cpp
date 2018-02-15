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

void Sensor::SetInputTypes(std::vector<Event::Type> list)
{
	this->mInputTypes = list;
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

	this->mpActiveEvent = this->mpEventLog->AddEvent(type, this->mRoomIndex);
	return this->mpActiveEvent->GetIndexInEventLog();
}

int Sensor::PlotTrigger(Event::Type type)
{
	this->mpActiveEvent = this->mpEventLog->AddEvent(type, this->mRoomIndex);
	return this->mpActiveEvent->GetIndexInEventLog();
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

void Sensor::SetActiveEvent(int index, ActiveEvent *pActiveEvent)
{
	this->mActiveEventIndex = index;
	this->mpActiveEvent = pActiveEvent;
}

int Sensor::GetActiveEventIndex() const
{
	return this->mActiveEventIndex;
}

ActiveEvent* Sensor::GetActiveEventPointer() const
{
	return this->mpActiveEvent;
}

std::vector<LogEvent*> Sensor::GetActiveEvents() const
{
	// Sanity check
	if (this->mpActiveEvent == nullptr)
	{
		std::vector<LogEvent*> emptyVector;
		return emptyVector;
	}

	return this->mpActiveEvent->GetActiveEvents();
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
