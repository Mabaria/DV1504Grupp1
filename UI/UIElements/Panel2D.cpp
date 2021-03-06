#include "Panel2D.h"
#include "../../IO/Mouse.h"
#include <string>
#include <iostream>
Panel2D::Panel2D(
	int width,
	int height,
	int top,
	int left,
	HWND handle,
	LPCTSTR title)
	:Panel(width, height, top, left, handle, title)
{
	this->mDirect2D->CreateRenderTarget(this->mPanelWindow, width, height);

	// Assumes the panel does not have a notification list.
	this->mNotificationList = nullptr;
	this->mNotificationListIsActive = false;
}

Panel2D::~Panel2D()
{
	for (unsigned int i = 0; i < this->mButtonVector.size(); i++)
	{
		if (this->mButtonVector[i])
		{
			delete this->mButtonVector[i];
		}
	}
	for (unsigned int i = 0; i < this->mTextBoxVector.size(); i++)
	{
		if (this->mTextBoxVector[i])
		{
			delete this->mTextBoxVector[i];
		}
	}
	if (this->mNotificationListIsActive)
	{
		delete this->mNotificationList;
	}
}

void Panel2D::AddButton(
	int width,
	int height,
	int top,
	int left,
	std::string imageFilePath,
	std::string buttonName)
{
	this->mButtonNames.push_back(buttonName); // Add to names list
	Button *newButton = new Button(this->mDirect2D,
		imageFilePath,
		left,
		top,
		left + width,
		top + height,
		buttonName);
	this->mButtonVector.push_back(newButton); // Add button
}

void Panel2D::AddButton(
	int width, 
	int height, 
	int top, 
	int left, 
	ID2D1Bitmap * bitmap, 
	std::string buttonName)
{
	this->mButtonNames.push_back(buttonName); // Add to names list
	Button *newButton = new Button(this->mDirect2D,
		bitmap,
		left,
		top,
		left + width,
		top + height,
		buttonName);
	this->mButtonVector.push_back(newButton); // Add button
}

void Panel2D::AddTextbox(
	int width,
	int height,
	int top,
	int left,
	std::string text,
	std::string name)
{
	this->mTextBoxNames.push_back(name); // Add name.

	TextBox *newTextBox = new TextBox(
		this->mDirect2D,
		left,
		top,
		left + width,
		top + height);
	this->mTextBoxVector.push_back(newTextBox); // Add text box.

	this->mTextBoxVector.back()->SetText(text); // Set text.
}

Button * Panel2D::GetButtonByName(std::string name)
{
	Button *to_return = nullptr; // Default return is nullptr
	unsigned int count = 0;
	std::vector<std::string>::iterator it;

	for (it = this->mButtonNames.begin();
		it != this->mButtonNames.end();
		++it, count++)
	{
		if (name.compare(*it) == 0) // Button with correct name found
		{
			to_return = this->mButtonVector[count]; // Return pointer to button
			it = this->mButtonNames.end() - 1; // Set iterator to end
											   // -1 because incrementation is performed after this.
											   // Incrementing on .end() is a baaad idea.
		}
	}
	return to_return;
}

Button * Panel2D::GetButtonByIndex(unsigned int index)
{
	Button *to_return = nullptr;
	if (index <= this->mButtonVector.size()) // Bounds check
	{
		to_return = this->mButtonVector[index];
	}
	return to_return;
}

int Panel2D::GetNrOfButtons() const
{
	return (int)this->mButtonVector.size();
}


TextBox * Panel2D::GetTextBoxByName(std::string name)
{
	TextBox *to_return = nullptr; // Default return is nullptr
	unsigned int count = 0;
	std::vector<std::string>::iterator it;

	for (it = this->mTextBoxNames.begin();
		it != this->mTextBoxNames.end();
		++it, count++)
	{
		if (name.compare(*it) == 0) // Button with correct name found
		{
			// Return pointer to button
			to_return = this->mTextBoxVector[count];

			// Set iterator to end
			// -1 because incrementation is performed after this.
			// Incrementing on .end() is a baaad idea.
			it = this->mTextBoxNames.end() - 1;
		}
	}
	return to_return;
}

TextBox * Panel2D::GetTextBoxByIndex(unsigned int index)
{
	TextBox *to_return = nullptr;
	if (index <= this->mTextBoxVector.size()) // Bounds check
	{
		to_return = this->mTextBoxVector[index];
	}
	return to_return;
}

void Panel2D::SetNotificationList(
	int posX,
	int posY,
	int titleFontSize,
	int objectFontSize)
{
	// Load the action symbols so we can send them to notification list
	this->LoadImageToBitmap("../../Models/Symbols.png", "Symbols");
	
	this->mNotificationList = new NotificationList(
		this->mDirect2D,
		posX,
		posY,
		titleFontSize,
		objectFontSize,
		this->GetBitmapByName("Symbols"));
	this->mNotificationListIsActive = true;
}

NotificationList * Panel2D::GetNotificationList()
{
	return this->mNotificationList;
}

bool Panel2D::AddNotification(Room * room, LogEvent * event, bool addedBySensor)
{
	ID2D1Bitmap *bitmap = nullptr;
	switch (event->GetType())
	{
	case Event::Fire:
		bitmap = this->GetBitmapByName("Fire");
		break;
	case Event::Water:
		bitmap = this->GetBitmapByName("Water");
		break;
	case Event::Gas:
		bitmap = this->GetBitmapByName("Gas");
		break;
	case Event::Injury:
		bitmap = this->GetBitmapByName("Injury");
		break;
	default:
		break;
	}

	bool result = this->mNotificationList->AddNotification(
		this->mDirect2D,
		room,
		event,
		bitmap,
		addedBySensor);
	return result;
}

bool Panel2D::RemoveNotification(Room * room, Event::Type type)
{
	return this->mNotificationList->RemoveNotification(room, type);
}

bool Panel2D::GetButtonOcclude()
{
	return this->mButtonOccludes;
}

void Panel2D::ScrollActiveLog()
{
	if (Mouse::GetScroll() != 0
		&& this->mNotificationList->GetListHeight()
	> this->mDirect2D->GetpRenderTarget()->GetSize().height
		&& this->IsMouseInsidePanel())
	{
		this->mNotificationList->MoveLog(Mouse::GetScroll() * 10.0f);
	}
}

void Panel2D::Scroll()
{
	float lowest_point	= 0.0f;
	float highest_point	= (float)INFINITY;

	// Calculate the highest and lowest point.
	for (int i = 0; i < (int)this->mTextBoxVector.size(); i++)
	{
		if (this->mTextBoxVector[i]->GetTextBoxSize().bottom > lowest_point)
		{
			lowest_point = this->mTextBoxVector[i]->GetTextBoxSize().bottom;
		}
		if (this->mTextBoxVector[i]->GetTextBoxSize().top < highest_point)
		{
			highest_point = this->mTextBoxVector[i]->GetTextBoxSize().top;
		}
	}

	if ((Mouse::GetScroll() != 0.0f) 
		&& (lowest_point + highest_point > this->mTop + this->mHeight))
	{
		float scroll_speed = Mouse::GetScroll() * 10.0f;
		// Move the boxes in the direction of the scroll.
		for (int i = 0; i < (int)mTextBoxVector.size(); i++)
		{
			this->mTextBoxVector[i]->MoveTextBox(0, (int)scroll_speed);
		}
		// If out of bounds, revert.
		if ((lowest_point < this->mTop + this->mHeight) 
			|| (highest_point > this->mTop))
		{
			for (int i = 0; i < (int)mTextBoxVector.size(); i++)
			{
				this->mTextBoxVector[i]->MoveTextBox(0, -(int)scroll_speed);
			}
		}
	}
}

void Panel2D::Update()
{
	this->UpdateWindowSize();
	this->mUpdateButtons();

	// Updating the notification list only if
	// the panel has one.
	if (this->mNotificationListIsActive
		&& this->mNotificationList->GetNumberOfNotificationObjects())
	{
		ScrollActiveLog();
		this->mNotificationList->Update();
	}	
}

void Panel2D::Draw()
{
	this->mDirect2D->GetpRenderTarget()->BeginDraw();
	this->mDirect2D->GetpRenderTarget()->Clear(
		D2D1::ColorF(D2D1::ColorF(
			0.3f,
			0.41f,
			0.5f,
			1.0f)));
	// Draw all the buttons in the panel
	for (std::vector<Button*>::iterator it =
		this->mButtonVector.begin();
		it != this->mButtonVector.end();
		it++)
	{
		(*it)->DrawButton();
	}
	// Draw all the text boxes in the panel
	for (std::vector<TextBox*>::iterator it =
		this->mTextBoxVector.begin();
		it != this->mTextBoxVector.end();
		it++)
	{
		(*it)->DrawTextBox();
	}
	// Draw all the notification objects if there is a list.
	if (this->mNotificationListIsActive)
	{
		this->mNotificationList->Draw();
	}


	this->mDirect2D->GetpRenderTarget()->EndDraw();
}

void Panel2D::mUpdateButtons()
{
	// For notification list.
	Button *button = nullptr;
	static Button *same_button_check = nullptr;
	NotificationObject *notification_object = nullptr;
	//----------------------
	this->mButtonOccludes = false;
	if (this->IsMouseInsidePanel()) /* Check if mouse is inside panel,
									if not there is no chance of any buttons
									being pressed. */
	{
		for (std::vector<Button*>::iterator it = this->mButtonVector.begin();
			it != this->mButtonVector.end();
			it++)
		{
			if ((*it)->GetAlive())
			{
				if (Mouse::GetPositionPercentage().x <
					(*it)->GetBoundingBoxPercentage().right &&
					Mouse::GetPositionPercentage().x >
					(*it)->GetBoundingBoxPercentage().left &&
					Mouse::GetPositionPercentage().y <
					(*it)->GetBoundingBoxPercentage().bottom &&
					Mouse::GetPositionPercentage().y >
					(*it)->GetBoundingBoxPercentage().top) /* Classic bounds check */
				{
					this->mButtonOccludes = true;
					if (Mouse::IsButtonPressed(Buttons::Left))
					{
						(*it)->SetButtonStatus(BUTTON_STATE::CLICKED);
						same_button_check = (*it);
					}
					else if (Mouse::IsButtonReleased(Buttons::Left))
					{
						if ((*it)->GetButtState() == BUTTON_STATE::CLICKED)
						{
							(*it)->SetButtonStatus(BUTTON_STATE::RELEASED);
							same_button_check = nullptr;
						}
					}
					else if (!Mouse::IsButtonDown(Buttons::Left) ||
						(*it)->GetButtState() != BUTTON_STATE::CLICKED)
					{
						if (same_button_check != nullptr &&
							same_button_check == (*it))
						{
							(*it)->SetButtonStatus(BUTTON_STATE::CLICKED);
						}
						else
						{
							(*it)->SetButtonStatus(BUTTON_STATE::HOVER);
						}

					}
				}
				else
				{
					(*it)->SetButtonStatus(BUTTON_STATE::IDLE);
					if (Mouse::IsButtonReleased(Buttons::Left))
					{
						same_button_check = nullptr;
					}
				}
			}
		}
		//! Notification list sets rect status instead of button status.
		// Notifies system if a notification object is clicked.
		if (this->mNotificationListIsActive)
		{
			for (int i = 0;
				i < this->mNotificationList->GetNumberOfNotificationObjects();
				i++)
			{
				notification_object = this->mNotificationList->
					GetNotificationObjectByIndex(i);
				button = notification_object->GetButton();

				if (Mouse::GetPositionPercentage().x <
					button->GetBoundingBoxPercentage().right &&
					Mouse::GetPositionPercentage().x >
					button->GetBoundingBoxPercentage().left &&
					Mouse::GetPositionPercentage().y <
					button->GetBoundingBoxPercentage().bottom &&
					Mouse::GetPositionPercentage().y >
					button->GetBoundingBoxPercentage().top)
					/* Classic bounds check */
				{
					this->mButtonOccludes = true;
					if (Mouse::IsButtonPressed(Buttons::Left))
					{
						button->SetRectStatus(BUTTON_STATE::CLICKED);
					}
					else if (Mouse::IsButtonReleased(Buttons::Left))
					{
						
						if (button->GetButtState() == BUTTON_STATE::CLICKED)
						{
							ObserverInfo obs_inf;
							obs_inf.pRoom = notification_object->GetRoom();
							obs_inf.actionData = No_Action;
							notification_object->NotifyObservers(&obs_inf);
							notification_object->SetIfNewStatus(false);
						}
					}
					else if (!Mouse::IsButtonDown(Buttons::Left) ||
						button->GetButtState() != BUTTON_STATE::CLICKED)
					{
						button->SetRectStatus(BUTTON_STATE::HOVER);
					}
				}
				else
				{
					button->SetRectStatus(BUTTON_STATE::IDLE);
				}

			}
		}
	}
	else // Mouse outside panel, make sure buttons are idle
	{
		for (std::vector<Button*>::iterator it = this->mButtonVector.begin();
			it != this->mButtonVector.end();
			it++)
		{
			(*it)->SetButtonStatus(BUTTON_STATE::IDLE);
		}
		if (this->mNotificationListIsActive)
		{
			for (int i = 0;
				i < this->mNotificationList->GetNumberOfNotificationObjects();
				i++)
			{
				this->mNotificationList->
					GetNotificationObjectByIndex(i)->
					GetButton()->
					SetRectStatus(BUTTON_STATE::IDLE);
			}
		}
	}
}
