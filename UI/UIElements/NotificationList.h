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
	NotificationList(int posX, int posY);
	~NotificationList();

	void AddNotification(Direct2D *direct2d, Room *room, LogEvent *event);
	bool RemoveNotification(Room *room, LogEvent *event);

	void Update();
	void Draw();

private:
	int mPosX;
	int mPosY;

	std::vector<NotificationObject*> mObjects;
};
