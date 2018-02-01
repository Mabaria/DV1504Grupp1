#include "ActiveEvent.h"

ActiveEvent::ActiveEvent()
{
	this->mRoomIndex = -1;
}

ActiveEvent::~ActiveEvent()
{
}



/**
*	Event specific
*/

bool ActiveEvent::AddEvent(int index)
{
	for (int i = 0; i < this->mEventIndices.size(); i++)
	{
		if (this->mEventIndices[i] == index)
			return false; // Event already exists
	}

	this->mEventIndices.push_back(index);
	return true;
}

bool ActiveEvent::ClearEvent(int index)
{
	for (int i = 0; i < this->mEventIndices.size(); i++)
	{
		if (this->mEventIndices[i] == index)
		{
			this->mEventIndices.erase(this->mEventIndices.begin() + i);
			return true;
		}
	}
	return false;
}

int ActiveEvent::GetEventCount() const
{
	return (int)this->mEventIndices.size();
}

int ActiveEvent::operator[](int index) const
{
	return this->mEventIndices[index];
}



/**
*	Room specific
*/

void ActiveEvent::SetRoomIndex(int index)
{
	this->mRoomIndex = index;
}

int ActiveEvent::GetRoomIndex() const
{
	return this->mRoomIndex;
}
