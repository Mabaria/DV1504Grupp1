#include "ActiveLog.h"

ActiveLog::ActiveLog()
{
}

ActiveLog::~ActiveLog()
{
}

void ActiveLog::AddEvent(int eventIndex, int roomIndex)
{
	int foundIndex = -1;

	for (int i = 0; i < this->mEventIndices.size() && foundIndex == -1; i++)
	{
		if (this->mEventIndices[i].GetRoomIndex() == roomIndex)
		{
			foundIndex = i;
		}
	}

	// If room already exist
	if (foundIndex != -1)
	{
		this->mEventIndices[foundIndex].AddEvent(eventIndex);
	}
	else
	{
		RoomEvent newRoomEvent(eventIndex);
		newRoomEvent.AddEvent(eventIndex);

		this->mEventIndices.push_back(newRoomEvent);
	}
}

void ActiveLog::RemoveEvent(int index)
{
	this->mEventIndices.erase(this->mEventIndices.begin() + index);
}

RoomEvent ActiveLog::operator[](int index)
{
	return this->mEventIndices[index];
}
