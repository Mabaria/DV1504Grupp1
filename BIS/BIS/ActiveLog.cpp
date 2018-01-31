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

	for (int i = 0; i < this->mActiveEvents.size() && index == -1; i++)
	{
		if (this->mActiveEvents[i].GetRoomIndex() == roomIndex)
		{
			index = i;
		}
	}

	// If room already exist
	if (index != -1)
	{
		this->mActiveEvents[index].AddEvent(eventIndex);
	}
	else
	{
		ActiveEvent newActiveEvent;

		newActiveEvent.SetRoomIndex(roomIndex);
		newActiveEvent.AddEvent(eventIndex);

		index = (int)this->mActiveEvents.size();
		this->mActiveEvents.push_back(newActiveEvent);
	}

	return index;
}

void ActiveLog::RemoveEvent(int index)
{
	this->mActiveEvents.erase(this->mActiveEvents.begin() + index);
}
