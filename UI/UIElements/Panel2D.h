#pragma once

#include <Windows.h>
#include <vector>
#include "Panel.h"
#include "Button.h"
#include "TextBox.h"
#include "NotificationList.h"
#include "../../GraphicsEngine/DX/Direct2D.h"

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

	void AddButton(
		int width,
		int height,
		int top,
		int left,
		ID2D1Bitmap* bitmap,
		std::string buttonName);

	Button* GetButtonByName(std::string name);
	Button* GetButtonByIndex(unsigned int index);
	ID2D1Bitmap* GetBitmapByName(std::string bitmapName);
	void LoadImageToBitmap(std::string imageFilePath, std::string bitmapName);


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
	void SetNotificationList(
		int posX, 
		int posY,
		int titleFontSize,
		int objectFontSize);

	// Messing up the coupling slightly to have 
	// system observe the notification objects.
	NotificationList *GetNotificationList();

	bool AddNotification(Room *room, LogEvent *event);
	bool RemoveNotification(Room *room, Event::Type type);

	bool GetButtonOcclude();

	void ScrollActiveLog();
	void Scroll();
	void Update();
	void Draw();

private:
	Direct2D *mDirect2D;
	
	void mUpdateButtons();

	std::vector<Button*> mButtonVector;
	std::vector<std::string> mButtonNames;
	std::vector<BitmapInfo> mBitmapVector;

	std::vector<TextBox*> mTextBoxVector;
	std::vector<std::string> mTextBoxNames;

	NotificationList *mNotificationList;
	bool mNotificationListIsActive;

	bool mButtonOccludes;


};
