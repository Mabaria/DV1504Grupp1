#pragma once
#include "Button.h"
#include "TextBox.h"
#include "../../Logic/Timer.h"
#include "../../Logic/Eventlog/LogEvent.h"
#include "../../Logic/Boat/Room.h"

class NotificationObject
{
public:
	NotificationObject(Room *room, LogEvent *event);
	~NotificationObject();
	
	const Event::Type GetEventType() const;
	const std::string GetRoomName() const;
	const std::string GetDeckName() const;
	const std::string GetElapsedTime() const;
	const std::string GetStartTime() const;

	const std::string GetNotificationString() const;

	void Update();

private:

	Timer *mTimer;

	std::string mRoomName;
	std::string mDeckName;

	std::string mStartTime;
	std::string mElapsedTime;

	Event::Type mEventType;
};
