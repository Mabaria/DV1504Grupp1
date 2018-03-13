#pragma once
#include "NotificationObject.h"

/*
* Notification list handles the notifications
* orders them by how long they have been active.
* 
* Handles adding and removing without leaving "gaps".
*/

class NotificationList
{
public:
	NotificationList(
		Direct2D *direct2d, 
		int posX, 
		int posY,
		int titleFontSize,
		int objectFontSize,
		ID2D1Bitmap* actionSymbolsBitmap);
	~NotificationList();

	bool AddNotification(
		Direct2D *direct2d, 
		Room *room, 
		LogEvent *event,
		ID2D1Bitmap *bitmap = nullptr,
		bool addedBySensor = false);

	bool RemoveNotification(Room *room, Event::Type type);
	NotificationObject *GetNotificationObjectByIndex(int index);
	const int GetNumberOfNotificationObjects() const;

	const int GetListHeight() const;


	void MoveLog(float speed);
	void Update();
	void UpdateBorders();
	void Draw();

private:
	int mPosX;
	int mPosY;
	ID2D1Bitmap* mpActionSymbolsBitmap;

	int mListTop;
	int mListBottom;

	int mSpace;
	int mObjectFontSize;

	TextBox mTitle;
	Button mTitleFrame;
	std::string mDefaultTitle;

	// Render target pointer for size purposes.
	ID2D1HwndRenderTarget *mpRenderTarget;
	std::vector<NotificationObject*> mObjects;
};
