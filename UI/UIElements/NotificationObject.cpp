#include "NotificationObject.h"

NotificationObject::NotificationObject()
{
}

NotificationObject::~NotificationObject()
{
}

void NotificationObject::AddButton(int width, int height, int top, int left, std::string imageFilePath, std::string buttonName)
{
	{
		this->mButtonNames.push_back(buttonName); // Add to names list
		Button newButton(this->mDirect2D,
			imageFilePath,
			left,
			top,
			left + width,
			top + height);
		this->mButtonVector.push_back(newButton); // Add button
	}
}

void NotificationObject::AddTextbox(int width, int height, int top, int left, std::string text, std::string name)
{
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

}
