#pragma once

#include <sstream>
#include "../Timer.h"
#include "../CorrectName.h"

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
		Injured_Treated,
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
		int *pActionIndex;
		float pos_x;
		float pos_z;
		bool start;
		bool active;

		/**
		*	0	-	No rotation
		*	16	-	90 degrees
		*	32	-	180 degrees
		*	48	-	270 degrees
		*	64	-	Stationary (depending on camera)
		*/
		uint32_t rotation; // 0-3 

		// Will be filled automatically
		std::string roomName;
		int ID;
	};

	static std::string GetStringFromType(LogAction::Type type);
	static LogAction::Type GetTypeFromString(std::string type);
	static LogAction::Type GetTypeFromNumber(int number);

	LogAction();
	LogAction(LogAction::Desc desc);
	LogAction(std::string lineFromLog, std::string metaLine);
	~LogAction();

	// Action specific
	void SetInactive();
	void SetActive();
	void SetType(LogAction::Type type);
	bool IsActive() const;
	LogAction::Type GetType() const;
	int* GetActionIndex();
	void SetPos(float x, float z);
	float GetPos_X() const;
	float GetPos_Z() const;
	int GetID() const;
	uint32_t GetRotation() const;

	// Log specific
	std::string GetLogString() const;
	std::string GetMetaString() const;

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
	bool mStart;
	LogAction::Type mType;
	std::string mRoomName;

	int *mpActionIndex;
	LogAction::Coordinate mCoord;

	// Used when saving/reading from file
	int mID;
	uint32_t mRotation;
};
