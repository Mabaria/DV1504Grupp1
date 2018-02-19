#include "NotificationList.h"

NotificationList::NotificationList(Direct2D *direct2d, int posX, int posY)
	:mTitle(direct2d, 0, 0, 0, 0), mTitleFrame(direct2d, "", 0, 0, 0, 0)
{
	this->mPosX = posX;
	this->mPosY = posY;
	this->mListTop		= this->mPosY;
	this->mListBottom	= this->mPosY;
	this->mpRenderTarget = direct2d->GetpRenderTarget();
	this->mSpace = 4;
	this->mTitle.SetTextBoxSize(
		this->mPosX, 
		this->mPosY, 
		(int)direct2d->GetpRenderTarget()->GetSize().width,
		(int)direct2d->GetpRenderTarget()->GetSize().height / 10);

	this->mDefaultTitle = "Aktiv Logg\n Antal: ";
	this->mTitle.SetText(this->mDefaultTitle + "0");
	this->mTitleFrame.SetButtonSize(
		0,
		0,
		(int)this->mTitle.GetTextBoxSize().right,
		(int)this->mTitle.GetTextBoxSize().bottom);
	this->mTitle.SetFontSize(40);
	this->mTitle.SetFontWeight(DWRITE_FONT_WEIGHT_ULTRA_BLACK);
	this->mTitle.SetFontName(L"times new roman");
	this->mTitle.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}

NotificationList::~NotificationList()
{
	for (int i = 0; i < (int)this->mObjects.size(); i++)
	{
		delete this->mObjects[i];
	}
}

bool NotificationList::AddNotification(
	Direct2D * direct2d, 
	Room * room, 
	LogEvent * event)
{
	bool new_event = true;
	for (int i = 0; i < (int)this->mObjects.size() && new_event; i++)
	{
		if ((room->GetName() == this->mObjects[i]->GetRoomName()) &&
			(room->GetDeckName() == this->mObjects[i]->GetDeckName()) &&
			(event->GetType() == this->mObjects[i]->GetEventType()))
		{
			new_event = false;
		}
	}
	if (new_event)
	{
		// Creates a new notification object and pushes it to the vector 
		// along with the number of objects in the vector for indexing.
		this->mObjects.push_back(new NotificationObject(
			room,
			event,
			direct2d,
			(int)this->mObjects.size() + 1));

		// Moves the object 2 pixels in x for looks, and in y based on the
		// number of objects in the list plus an offset for looks plus the
		// height of the title text box.
		this->mObjects.back()->Move(
			2,
			((int)this->mObjects.size() - 1)
			* (this->mObjects[0]->GetHeight() + this->mSpace)
			+ (int)this->mTitle.GetTextBoxSize().bottom);

		// Updates the number of events in the title.
		this->mTitle.SetText(
			this->mDefaultTitle + std::to_string(this->mObjects.size()));
	}
	return new_event;
}

bool NotificationList::RemoveNotification(Room * room, Event::Type type)
{
	// Returns false if the notification object is not found.
	bool result = false;

	std::string room_name = room->GetName();
	std::string deck_name = room->GetDeckName();

	// Searching through the vector until 
	// a matching notification object is found.
	for (int i = 0; i < (int)this->mObjects.size(); i++)
	{
		if ((room_name == this->mObjects[i]->GetRoomName()) &&
			(deck_name == this->mObjects[i]->GetDeckName()) &&
			(type == this->mObjects[i]->GetEventType()))
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
				this->mObjects[j]->SetIndex(this->mObjects[j]->GetIndex() - 1);
			}

			// Deleting the last object of the vector
			// as that will be a duplicate of the second
			// to last object at this point.
			this->mObjects.pop_back();

			// Updates the number of events in the title.
			this->mTitle.SetText(
				this->mDefaultTitle + std::to_string(this->mObjects.size()));
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
	
	this->UpdateBorders();

	if (this->mListTop > this->mSpace + this->mTitle.GetTextBoxSize().bottom)
	{
		for (int i = 0; i < (int)this->mObjects.size(); i++)
		{
			this->mObjects[i]->Move(0, 
				this->mSpace 
				- this->mListTop 
				+ (int)this->mTitle.GetTextBoxSize().bottom);
		}
	}
	else if (this->mListBottom < this->mpRenderTarget->GetSize().height)
	{
		for (int i = 0; i < (int)this->mObjects.size(); i++)
		{
			this->mObjects[i]->Move(0, 
				(int)this->mpRenderTarget->GetSize().height
				- this->mListBottom - this->mSpace);
		}
	}
}

void NotificationList::Update()
{
	// Updating time for the notification objects.
	for (int i = 0; i < (int)this->mObjects.size(); i++)
	{
		this->mObjects[i]->Update();
	}

	// List top and list bottom
	this->UpdateBorders();
}

void NotificationList::UpdateBorders()
{
	// Top y value for the first object of the list.
	this->mListTop = (int)
		this->mObjects.front()->
		GetButton()->
		GetButtonSize().
		top;
	// Bottom y value for the last object of the list.
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
		//! O P T I M I Z A T I O N
		if (((int)this->mObjects[i]->GetButton()->GetButtonSize().bottom 
			> this->mTitle.GetTextBoxSize().bottom)
			&& (int)this->mObjects[i]->GetButton()->GetButtonSize().top
			< (int)this->mpRenderTarget->GetSize().height)
		{
			this->mObjects[i]->Draw();
		}
	}
	this->mTitleFrame.DrawFilledRect(0.9f, 0.9f, 0.9f, 1.0f);
	this->mTitle.DrawTextBox();
}
