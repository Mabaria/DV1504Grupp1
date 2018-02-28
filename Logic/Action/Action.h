#pragma once

#include "../Timer.h"


enum ActionType
{
	Injured_Moved,
	Injured_Reported,
	Hole_In_Bulk,
	Ventilation_In,
	Ventilation_Out,
	Cooling_Wall,
	Supporting_Wall,
	Damaged_Bulk
};

class Action
{
public:

	Action();
	Action(ActionType type, std::string roomName);
	~Action();

	// Action specific
	void SetType(ActionType type);
	ActionType GetType() const;

	// Room specific
	void SetRoomName(std::string name);
	std::string GetRoomName() const;

	// Time specific
	const int GetElapsedTime();
	const std::string GetStartTimeAsString();
	const std::string GetElapsedTimeAsString();
	Timestamp GetTimestamp_Start() const;

private:

	ActionType mType;
	Timer mTimer;
	std::string mRoomName;
};
