#pragma once
#include "Button.h"
#include "TextBox.h"
#include "../../Logic/Timer.h"
#include "../../Logic/Log/LogEvent.h"
#include "../../Logic/Boat/Room.h"
#include "../../UI/UIElements/Button.h"
#include "../../UI/UIElements/TextBox.h"
#include "../../GraphicsEngine/DX/Direct2D.h"
#include "../../GraphicsEngine/Actions.h"


#ifndef OBSERVER_INFO
#define OBSERVER_INFO
struct ObserverInfo
{
	Room *pRoom = nullptr;
	uint32_t actionData;
};
#endif

class NotificationObject : public Observable<ObserverInfo>
{
public:
	NotificationObject(
		Room *room, 
		LogEvent *event,
		Direct2D *direct2d, 
		int index,
		int fontSize,
		ID2D1Bitmap *bitmap = nullptr,
		ID2D1Bitmap *actionSymbolsBitmap = nullptr);
		ID2D1Bitmap *bitmap = nullptr,
		bool addedBySensor = false);
	~NotificationObject();
	
	const Event::Type GetEventType() const;
	const std::string GetRoomName() const;
	const std::string GetDeckName() const;
	const std::string GetElapsedTime() const;
	const std::string GetStartTime() const;

	const std::string GetNotificationString() const;

	Button *GetButton();
	Room *GetRoom();

	const int GetWidth() const;
	const int GetHeight() const;
	const int GetBottom() const;

	// For changing the index upon deletion.
	const int GetIndex() const;
	const void SetIndex(int index);

	const void Move(int x, int y);
	const void SetText(std::string text);
	const void SetIfNewStatus(bool status);
	const bool GetIfNewStatus();
	const bool GetIfAddedBySensor();

	void Update();
	void Draw();

private:
	Button mButton;
	Button mActionButton;
	TextBox mTextBox;
	Timer *mpTimer;
	Room *mpRoom;

	bool mIsNew;
	float mNewColorCounter;
	bool mAddedBySensor;

	std::string mRoomName;
	std::string mDeckName;

	std::string mStartTime;
	std::string mElapsedTime;

	ID2D1Bitmap *mpActionSymbolsBitmap;

	void mUpdateActionButton();
	bool mFindActionType(std::vector<LogAction*> &actionVector, LogAction::Type typeToFind);

	int mIndex;

	Event::Type mEventType;
};
