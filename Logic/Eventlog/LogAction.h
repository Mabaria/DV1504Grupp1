#pragma once

#include "../Timer.h"




class LogAction
{
public:

	// Can be accessed with LogAction::Coordinate
	struct Coordinate
	{
		float x;
		float z;
	};

	enum Type
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

	struct Desc
	{
		// Fill these
		LogAction::Type type;
		int actionIndex;
		float pos_x;
		float pos_z;

		// This one fills automatically
		std::string roomName;
	};

	LogAction();
	LogAction(LogAction::Desc desc);
	~LogAction();

	// Action specific
	void SetInactive();
	void SetType(LogAction::Type type);
	bool IsActive() const;
	LogAction::Type GetType() const;

	int GetActionIndex() const;

	// Room specific
	void SetRoomName(std::string name);
	std::string GetRoomName() const;

	// Time specific
	const int GetElapsedTime();
	const std::string GetStartTimeAsString();
	const std::string GetElapsedTimeAsString();
	Timestamp GetTimestamp_Start() const;


private:

	Timer mTimer;

	bool mActive;
	LogAction::Type mType;
	std::string mRoomName;

	int mActionIndex;
	float mPos_x;
	float mPos_z;
};
