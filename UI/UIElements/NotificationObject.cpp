#include "NotificationObject.h"

NotificationObject::NotificationObject(
	Room *room,
	LogEvent *event,
	Direct2D *direct2d,
	int index,
	int fontSize,
	ID2D1Bitmap *bitmap,
	ID2D1Bitmap *actionSymbolsBitmap)
	ID2D1Bitmap *bitmap,
	bool addedBySensor)
	: mButton(direct2d, bitmap, 0, 0, 0, 0)
	, mActionButton(direct2d, actionSymbolsBitmap, 230, 7, 290, 67, "ActionButton")
	, mTextBox(direct2d, 0, 0, 0, 0)
{
	this->mpRoom = room;
	this->mpTimer = event->GetTimer();
	this->mRoomName = this->mpRoom->GetName();
	this->mDeckName = this->mpRoom->GetDeckName();
	this->mStartTime = this->mpTimer->WhenTimerStarted();
	this->mElapsedTime = this->mpTimer->GetTimeAsStr();
	this->mEventType = event->GetType();
	this->mIndex = index;
	this->mIsNew = true;
	this->mNewColorCounter = 0.0f;
	this->mAddedBySensor = addedBySensor;
	this->mpActionSymbolsBitmap = actionSymbolsBitmap;
	
	this->mTextBox.SetFontSize(fontSize);
	this->mTextBox.SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
	this->mTextBox.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	D2D1_SIZE_F render_target_size = direct2d->GetpRenderTarget()->GetSize();	

	// 200x80 notification object.
	D2D1_RECT_F object_size;
	object_size.top = 0;
	object_size.left = 0;
	object_size.right = render_target_size.width - 4;
	object_size.bottom = (float)fontSize * 8 / 3 + 5;

	// 40x40 pixel icon.
	D2D1_RECT_F icon_size;
	icon_size.top = 5;
	icon_size.left = 5;
	icon_size.right = object_size.bottom - 5;
	icon_size.bottom = object_size.bottom - 5;

	// 150x80 text box.
	D2D1_RECT_F textbox_size;
	textbox_size.top = icon_size.top;
	textbox_size.left = icon_size.right + 5;
	textbox_size.right = 19 * render_target_size.width / 20;
	textbox_size.bottom = object_size.bottom;

	if (!bitmap)
	{
		std::string file_path = "../../Models/";
		switch (this->mEventType)
		{
		case Event::Type::Fire:
			file_path += "Button04.png";
			break;
		case Event::Type::Water:
			file_path += "Button03.png";
			break;
		case Event::Type::Gas:
			file_path += "Button02.png";
			break;
		case Event::Type::Injury:
			file_path += "Button01.png";
			break;
		default:
			file_path += "pepehands.jpg";
		}

		this->mButton.CreateButton(file_path, 0, 0, 0, 0);
	}

	// Every notification object starts at the same place,
	// the handler places them where they need to be.
	this->mButton.SetButtonSize(
		(int)object_size.left,
		(int)object_size.top,
		(int)object_size.right,
		(int)object_size.bottom);

	this->mButton.SetIconSize(
		(int)icon_size.left,
		(int)icon_size.top,
		(int)icon_size.right,
		(int)icon_size.bottom);

	this->mTextBox.SetTextBoxSize(
		(int)textbox_size.left,
		(int)textbox_size.top,
		(int)textbox_size.right,
		(int)textbox_size.bottom);

	this->mTextBox.SetText(this->GetNotificationString());
	this->mButton.SetButtonStatus(BUTTON_STATE::IDLE);
	this->mActionButton.SetButtonStatus(BUTTON_STATE::IDLE);
	this->mActionButton.SetOpacity(0.0f);
	D2D1_SIZE_F action_button_size = actionSymbolsBitmap->GetSize();
	int step = (int)action_button_size.width / 4;
	this->mActionButton.SetBitmapRenderSize(
		step * 2,
		step,
		step * 3,
		step * 2
	);
}

NotificationObject::~NotificationObject()
{
}

const Event::Type NotificationObject::GetEventType() const
{
	return this->mEventType;
}

const std::string NotificationObject::GetRoomName() const
{
	return this->mRoomName;
}

const std::string NotificationObject::GetDeckName() const
{
	return this->mDeckName;
}

const std::string NotificationObject::GetElapsedTime() const
{
	return this->mElapsedTime;
}

const std::string NotificationObject::GetStartTime() const
{
	return this->mStartTime;
}

const std::string NotificationObject::GetNotificationString() const
{
	std::string return_string = "";
	return_string += std::to_string(this->mIndex);
	return_string += ". ";
	return_string += this->mRoomName;
	return_string += "\n";
	return_string += this->mElapsedTime;
	
	return return_string;
}

Button * NotificationObject::GetButton()
{
	return &this->mButton;
}

Room * NotificationObject::GetRoom()
{
	return this->mpRoom;
}

const int NotificationObject::GetWidth() const
{
	return (int)this->mButton.GetButtonSize().right 
		-  (int)this->mButton.GetButtonSize().left;
}

const int NotificationObject::GetHeight() const
{
	return (int)this->mButton.GetButtonSize().bottom
		-  (int)this->mButton.GetButtonSize().top;
}

const int NotificationObject::GetBottom() const
{
	return (int)this->mButton.GetButtonSize().bottom;
}

const int NotificationObject::GetIndex() const
{
	return this->mIndex;
}

const void NotificationObject::SetIndex(int index)
{
	this->mIndex = index;
}

const void NotificationObject::Move(int x, int y)
{
	this->mButton.MoveButton(x, y);
	this->mActionButton.MoveButton(x, y);
	this->mTextBox.MoveTextBox(x, y);
}

const void NotificationObject::SetText(std::string text)
{
	this->mTextBox.SetText(text);
}

const void NotificationObject::SetIfNewStatus(bool status)
{
	this->mIsNew = status;
}

const bool NotificationObject::GetIfNewStatus()
{
	return this->mIsNew;
}

const bool NotificationObject::GetIfAddedBySensor()
{
	return this->mAddedBySensor;
}

void NotificationObject::Update()
{
	this->mElapsedTime = this->mpTimer->GetTimeAsStr();
	this->SetText(this->GetNotificationString());
	this->mUpdateActionButton();
}

void NotificationObject::Draw()
{
	if (this->mIsNew && this->mAddedBySensor)
	{
		this->mButton.SetRectColor(
			sin(this->mNewColorCounter += 0.2f),
			0, 0);
	}

	this->mButton.DrawRect();
	this->mTextBox.DrawTextBox();
	this->mButton.DrawButton();

	if (this->mActionButton.GetOpacity() > 0.0f)
	{
		this->mActionButton.DrawRect();
		this->mActionButton.DrawButton();
	}
}

void NotificationObject::mUpdateActionButton()
{
	static int stride = (int)this->mpActionSymbolsBitmap->GetSize().width / 4;
	std::vector<LogAction*> active_actions;
	this->mpRoom->GetActiveActions(active_actions);
	this->mActionButton.SetOpacity(0.0f);
	if (active_actions.size() > 0) // If there are actions in the room
	{
		bool action_found = false;
		// Actions found in room, determine if we should place the action icon on the object
		switch (mEventType) {
		case Event::Type::Fire: {
			action_found = this->mFindActionType(active_actions, LogAction::Type::Cooling_Wall);
			if (action_found) {
				this->mActionButton.SetBitmapRenderSize(2 * stride, stride, 3 * stride, 2 * stride);
				this->mActionButton.SetOpacity(1.0f);
			}
			break;
		}
		case Event::Type::Gas: {
			action_found = this->mFindActionType(active_actions, LogAction::Type::Ventilation_Out);
			if (!action_found) // No vent out, check vent in
				action_found = this->mFindActionType(active_actions, LogAction::Type::Ventilation_In);

			if (action_found) {
				this->mActionButton.SetBitmapRenderSize(stride, stride, 2 * stride, 2 * stride);
				this->mActionButton.SetOpacity(1.0f);
			}
			break;
		}
		case Event::Type::Water: {
			// Todo ADD LÄNSNING
			action_found = this->mFindActionType(active_actions, LogAction::Type::Draining);
			if (action_found) {
				this->mActionButton.SetBitmapRenderSize(stride, 2 * stride, 2 * stride, 3 * stride);
				this->mActionButton.SetOpacity(1.0f);
			}
			break;
		}
		}
	}
}

bool NotificationObject::mFindActionType(std::vector<LogAction*>& actionVector, LogAction::Type typeToFind)
{
	bool to_return = false; // Assume failed find
	
	for (unsigned int i = 0; i < actionVector.size() && to_return == false; i++)
	{
		if (actionVector[i]->GetType() == typeToFind)
			to_return = true;
	}

	return to_return;
}
