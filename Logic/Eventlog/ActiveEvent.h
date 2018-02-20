#pragma once

#include <vector>
#include "LogEvent.h"

#include "../../IO/ObserverPattern/Observable.h"


class ActiveEvent : public Observable<ActiveEvent>
{
public:

	ActiveEvent();
	~ActiveEvent();
	
	// Event specific
	bool AddEvent(int index, LogEvent* pEvent);
	bool ClearEvent(Event::Type type);

	bool EventTypeExists(Event::Type type) const;

	bool IsEmpty() const;
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
