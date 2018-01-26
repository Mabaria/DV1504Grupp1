#include "ActiveLog.h"

ActiveLog::ActiveLog()
{
}

ActiveLog::~ActiveLog()
{
}

int ActiveLog::AddEvent(int eventIndex, int roomIndex)
{
	int index = -1;

	for (int i = 0; i < this->mRoomEvents.size() && index == -1; i++)
	{
		if (this->mRoomEvents[i].GetRoomIndex() == roomIndex)
		{
			index = i;
		}
	}

	// If room already exist
	if (index != -1)
	{
		this->mRoomEvents[index].AddEvent(eventIndex);
	}
	else
	{
		RoomEvent newRoomEvent;

		newRoomEvent.SetRoomIndex(roomIndex);
		newRoomEvent.AddEvent(eventIndex);

		index = (int)this->mRoomEvents.size();
		this->mRoomEvents.push_back(newRoomEvent);
	}

	return index;
}

void ActiveLog::RemoveEvent(int index)
{
	this->mRoomEvents.erase(this->mRoomEvents.begin() + index);
}

//RoomEvent ActiveLog::operator[](int index)
//{
//	return this->mRoomEvents[index];
//}
