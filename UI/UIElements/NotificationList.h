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
	NotificationList(Direct2D *direct2d, int posX, int posY);
	~NotificationList();

	void AddNotification(Direct2D *direct2d, Room *room, LogEvent *event);
	bool RemoveNotification(Room *room, LogEvent *event);
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

	int mListTop;
	int mListBottom;

	int mSpace;

	// Render target pointer for size purposes.
	ID2D1HwndRenderTarget *mRenderTarget;
	std::vector<NotificationObject*> mObjects;
};
