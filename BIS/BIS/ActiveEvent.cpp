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

void ActiveEvent::AddEvent(int index)
{
	this->mEventIndices.push_back(index);
}

void ActiveEvent::RemoveEvent(int index)
{
	for (int i = 0; i < this->mEventIndices.size(); i++)
	{
		if (this->mEventIndices[i] == index)
		{
			this->mEventIndices.erase(this->mEventIndices.begin() + i);
			return;
		}
	}
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
