#pragma once

#include <vector>
#include "LogEvent.h"


class ActiveEvent
{
public:

	ActiveEvent();
	~ActiveEvent();
	
	// Event specific
	bool AddEvent(int index, LogEvent* pEvent);
	bool ClearEvent(int index);

	bool EventTypeExists(Event::Type type) const;

	int GetEventCount() const;
	int GetEventIndexAt(int index) const;
	std::vector<LogEvent*> GetActiveEvents() const;

	void SetIndexInEventLog(int index);
	int GetIndexInEventLog() const;

	// Room specific
	void SetRoomIndex(int index);
	int GetRoomIndex() const;


private:

	struct EventInfo
	{
		int index;
		LogEvent *pointer;
	};
	
	// Event specific
	std::vector<EventInfo> mEvents;
	int mIndexInEventLog;

	// Room specific
	int mRoomIndex;
};
