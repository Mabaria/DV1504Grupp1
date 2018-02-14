#pragma once

#include <Windows.h>
#include "Panel.h"
#include "Button.h"
#include "../../GraphicsEngine/DX/Direct2D.h"
#include <vector>
#include "TextBox.h"
#include "NotificationList.h"

class Panel2D : public Panel
{
public:
	Panel2D(
		int width, 
		int height, 
		int top, 
		int left, 
		HWND handle, 
		LPCTSTR title);
	~Panel2D();

	// Button stuff.
	void AddButton(
		int width,
		int height,
		int top,
		int left,
		std::string imageFilePath,
		std::string buttonName);

	Button* GetButtonByName(std::string name);
	Button* GetButtonByIndex(unsigned int index);

	// Text box stuff.
	void AddTextbox(
		int width, 
		int height, 
		int top, 
		int left, 
		std::string text,
		std::string name);

	TextBox* GetTextBoxByName(std::string name);
	TextBox* GetTextBoxByIndex(unsigned int index);

	// Notification object/list stuff.
	void SetNotificationList(int posX, int posY);
	void AddNotification(Room *room, LogEvent *event);
	bool RemoveNotification(Room *room, LogEvent *event);


	void ScrollActiveLog();
	void Update();
	void Draw();

private:
	Direct2D *mDirect2D;
	
	void mUpdateButtons();

	std::vector<Button*> mButtonVector;
	std::vector<std::string> mButtonNames;

	void mUpdateTextBoxes();
	std::vector<TextBox*> mTextBoxVector;
	std::vector<std::string> mTextBoxNames;

	NotificationList *mNotificationList;
	bool mNotificationListIsActive;
};
