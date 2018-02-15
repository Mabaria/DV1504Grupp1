#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "../Event/Event.h"
#include "../Eventlog/EventLog.h"

#include "../../IO/ObserverPattern/Observer.h"

class Sensor : public Observer<ActiveEvent>
{
public:

	Sensor();
	~Sensor();

	// Sensor specific
	void AddInputType(Event::Type type);
	void RemoveInputType(Event::Type type);
	void SetInputTypes(std::vector<Event::Type> list);
	bool CanDetect(Event::Type type) const;
	
	bool AutoTrigger(Event::Type type); // Will return a ActiveEvent index
	bool PlotTrigger(Event::Type type);

	bool ClearEvent(Event::Type type);


	// Log specific
	void SetEventLog(EventLog *pEventLog);

	// Event specific
	void SetActiveEvent(int index, ActiveEvent *pActiveEvent);
	int GetActiveEventIndex() const;
	ActiveEvent* GetActiveEventPointer() const;
	std::vector<LogEvent*> GetActiveEvents() const;

	// Room specific
	void SetRoomIndex(int index);
	
	// Disk specific
	std::string WriteString() const;

	// Observer specific
	void Update(ActiveEvent *pAttribute) override;

private:

	// Sensor specific
	std::vector<Event::Type> mInputTypes;	/* Holds all the type of events the
											   sensor can detect */
	// Log specific
	EventLog *mpEventLog;

	// Event specific
	int mActiveEventIndex;
	ActiveEvent *mpActiveEvent;
	
	// Room specific
	int mRoomIndex;	// Index to the room this sensor is in
};
