#include "Panel2D.h"
#include "../../IO/Mouse.h"
#include <string>

Panel2D::Panel2D(
	int width, 
	int height, 
	int top, 
	int left, 
	HWND handle, 
	LPCTSTR title)
	:Panel(width, height, top, left, handle, title)
{
	this->mDirect2D = new Direct2D(mPanelWindow, width, height);

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
	for (unsigned int i = 0; i < this->mBitmapVector.size(); i++)
	{
		if (this->mBitmapVector[i].bitmap)
		{
			this->mBitmapVector[i].bitmap->Release();
			this->mBitmapVector[i].bitmap = nullptr;
		}
	}
	delete this->mDirect2D;

	// Deleting the notification list only if 
	// there is one to delete.
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

void Panel2D::AddButton(int width, int height, int top, int left, ID2D1Bitmap * bitmap, std::string buttonName)
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

ID2D1Bitmap * Panel2D::GetBitmapByName(std::string bitmapName)
{
	ID2D1Bitmap *to_return = nullptr; // Default return is nullptr
	std::vector<BitmapInfo>::iterator it;

	for (it = this->mBitmapVector.begin();
		it != this->mBitmapVector.end();
		++it)
	{
		if (bitmapName.compare((*it).name) == 0) // Button with correct name found
		{
			to_return = (*it).bitmap; // Return pointer to button
			it = this->mBitmapVector.end() - 1; // Set iterator to end
											   // -1 because incrementation is performed after this.
											   // Incrementing on .end() is a baaad idea.
		}
	}
	return to_return;
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

void Panel2D::SetNotificationList(int posX, int posY)
{
	this->mNotificationList = new NotificationList(
		this->mDirect2D, 
		posX, 
		posY);
	this->mNotificationListIsActive = true;
}

void Panel2D::AddNotification(Room * room, LogEvent * event)
{
	this->mNotificationList->AddNotification(
		this->mDirect2D, 
		room, 
		event);
}

bool Panel2D::RemoveNotification(Room * room, LogEvent * event)
{
	return this->mNotificationList->RemoveNotification(room, event);
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

void Panel2D::Update()
{
	this->UpdateWindowSize();
	this->mUpdateButtons();
	this->mUpdateTextBoxes();

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
			0.9f,
			0.9f,
			0.9f,
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
	if (this->IsMouseInsidePanel()) /* Check if mouse is inside panel,
									 if not there is no chance of any buttons
									 being pressed. */
	{
		for (std::vector<Button*>::iterator it = this->mButtonVector.begin();
			it != this->mButtonVector.end();
			it++)
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
				if (Mouse::IsButtonDown(Buttons::Left))
				{
					(*it)->SetButtonStatus(BUTTON_STATE::CLICKED);
				}
				else
					(*it)->SetButtonStatus(BUTTON_STATE::HOVER);
			}
			else
			{
				(*it)->SetButtonStatus(BUTTON_STATE::IDLE);
			}

		}
		//! Notification list sets rect status instead of button status.
		if (this->mNotificationListIsActive)
		{
			for (int i = 0;
				i < this->mNotificationList->GetNumberOfNotificationObjects();
				i++)
			{
				button = this->mNotificationList->
					GetNotificationObjectByIndex(i)->
					GetButton();

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
					if (Mouse::IsButtonDown(Buttons::Left))
					{
						button->SetRectStatus(BUTTON_STATE::CLICKED);
					}
					else
						button->SetRectStatus(BUTTON_STATE::HOVER);
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

void Panel2D::mUpdateTextBoxes()
{

}

void Panel2D::LoadImageToBitmap(
	std::string imageFilePath,
	std::string bitmapName)
{
	BitmapInfo new_bitmap_struct;
	new_bitmap_struct.name = bitmapName;
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> convert;
	std::wstring w_file_path = convert.from_bytes(imageFilePath);

	IWICFormatConverter *converter = this->mDirect2D->GetpFormatConverter();
	IWICBitmapDecoder *decoder = this->mDirect2D->GetpBitmapDecoder();
	IWICBitmapFrameDecode *bitmapSrc = this->mDirect2D->GetpBitmapSrc();

	this->mDirect2D->GetpImagingFactory()->CreateFormatConverter(&converter);
	this->mDirect2D->SetpFormatConverter(converter);
	this->mDirect2D->GetpImagingFactory()->CreateDecoderFromFilename(
		w_file_path.c_str(),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&decoder);
	this->mDirect2D->SetpBitmapDecoder(decoder);
	if (this->mDirect2D->GetpBitmapDecoder() != nullptr)
	{
		this->mDirect2D->GetpBitmapDecoder()->GetFrame(0, &bitmapSrc);
		this->mDirect2D->SetpBitmapSrc(bitmapSrc);
		this->mDirect2D->GetpFormatConverter()->Initialize(
			bitmapSrc,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut);
		this->mDirect2D->GetpRenderTarget()->CreateBitmapFromWicBitmap(
			this->mDirect2D->GetpFormatConverter(),
			NULL,
			&new_bitmap_struct.bitmap);
	}

	this->mBitmapVector.push_back(new_bitmap_struct);
}
