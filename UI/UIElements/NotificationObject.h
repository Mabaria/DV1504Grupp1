#pragma once
#include "Button.h"
#include "TextBox.h"
#include "../../Logic/Timer.h"
#include "../../Logic/Eventlog/LogEvent.h"
#include "../../Logic/Boat/Room.h"
#include "../../UI/UIElements/Button.h"
#include "../../UI/UIElements/TextBox.h"
#include "../../GraphicsEngine/DX/Direct2D.h"

class NotificationObject
{
public:
	NotificationObject(
		Room *room, 
		LogEvent *event,
		Direct2D *direct2d, 
		int index,
		int fontSize,
		ID2D1Bitmap *bitmap = nullptr);
	~NotificationObject();
	
	const Event::Type GetEventType() const;
	const std::string GetRoomName() const;
	const std::string GetDeckName() const;
	const std::string GetElapsedTime() const;
	const std::string GetStartTime() const;

	const std::string GetNotificationString() const;

	Button *GetButton();

	const int GetWidth() const;
	const int GetHeight() const;
	const int GetBottom() const;

	// For changing the index upon deletion.
	const int GetIndex() const;
	const void SetIndex(int index);

	const void Move(int x, int y);
	const void SetText(std::string text);

	void Update();
	void Draw();

private:
	Button mButton;
	TextBox mTextBox;
	Timer *mTimer;

	std::string mRoomName;
	std::string mDeckName;

	std::string mStartTime;
	std::string mElapsedTime;

	int mIndex;

	Event::Type mEventType;
};
