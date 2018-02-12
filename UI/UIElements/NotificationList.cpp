#include "NotificationList.h"

NotificationList::NotificationList(Direct2D *direct2d, int posX, int posY)
{
	this->mPosX = posX;
	this->mPosY = posY;
	this->mListTop		= this->mPosY;
	this->mListBottom	= this->mPosY;
	this->mRenderTarget = direct2d->GetpRenderTarget();
	this->mSpace = 4;
}

NotificationList::~NotificationList()
{
	for (int i = 0; i < (int)this->mObjects.size(); i++)
	{
		delete this->mObjects[i];
	}
}

void NotificationList::AddNotification(
	Direct2D * direct2d, 
	Room * room, 
	LogEvent * event)
{
	this->mObjects.push_back(new NotificationObject(room, event, direct2d));
	this->mObjects.back()->Move(
		2, 
		((int)this->mObjects.size() - 1)
		*
		(this->mObjects[0]->GetHeight() + this->mSpace) + this->mSpace);
}

bool NotificationList::RemoveNotification(Room * room, LogEvent * event)
{
	// Returns false if the notification object is not found.
	bool result = false;

	std::string room_name = room->GetName();
	std::string deck_name = room->GetDeckName();
	Event::Type event_type = event->GetType();

	// Searching through the vector until 
	// a matching notification object is found.
	for (int i = 0; i < (int)this->mObjects.size(); i++)
	{
		if ((room_name == this->mObjects[i]->GetRoomName()) &&
			(deck_name == this->mObjects[i]->GetDeckName()) &&
			(event_type == this->mObjects[i]->GetEventType()))
		{
			result = true;

			// Deleting that notification object.
			delete this->mObjects[i];

			// Going through the rest, pushing them one step back and moving
			// them one step up to fill the gap left by the deleted object.
			for (int j = i; j < (int)this->mObjects.size() - 1; j++)
			{
				this->mObjects[j] = this->mObjects[j + 1];
				this->mObjects[j]->Move(
					0,
					-this->mObjects[0]->GetHeight() - this->mSpace);
			}

			// Deleting the last object of the vector
			// as that will be a duplicate of the second
			// to last object at this point.
			this->mObjects.pop_back();
		}
	}
	return result;
}

NotificationObject *NotificationList::GetNotificationObjectByIndex(int index)
{
	if (index < (int)this->mObjects.size())
	{
		return this->mObjects[index];
	}
	return nullptr;
}

const int NotificationList::GetNumberOfNotificationObjects() const
{
	return this->mObjects.size();
}

const int NotificationList::GetListHeight() const
{
	return this->mListBottom - this->mListTop;
}

void NotificationList::MoveLog(float speed)
{
	
	for (int i = 0; i < (int)this->mObjects.size(); i++)
	{
		this->mObjects[i]->Move(0, (int)speed);
	}
	
	
	for (int i = 0; i < (int)this->mObjects.size(); i++)
	{
		this->mObjects[i]->Move(0, (int)speed);
	}
	
	this->UpdateBorders();

	if (this->mListTop > this->mSpace)
	{
		for (int i = 0; i < (int)this->mObjects.size(); i++)
		{
			this->mObjects[i]->Move(0, this->mSpace - this->mListTop);
		}
	}
	else if (this->mListBottom < this->mRenderTarget->GetSize().height)
	{
		for (int i = 0; i < (int)this->mObjects.size(); i++)
		{
			this->mObjects[i]->Move(0, 
				(int)this->mRenderTarget->GetSize().height
				- this->mListBottom - this->mSpace);
		}
	}

}

void NotificationList::Update()
{
	for (int i = 0; i < (int)this->mObjects.size(); i++)
	{
		this->mObjects[i]->Update();
	}
	this->UpdateBorders();
}

void NotificationList::UpdateBorders()
{
	this->mListTop = (int)
		this->mObjects.front()->
		GetButton()->
		GetButtonSize().
		top;
	this->mListBottom = (int)
		this->mObjects.back()->
		GetButton()->
		GetButtonSize().
		bottom;
}

void NotificationList::Draw()
{
	for (int i = 0; i < (int)this->mObjects.size(); i++)
	{
		this->mObjects[i]->Draw();
	}
}
