#pragma once

#include <Windows.h>
#include "Panel.h"
#include "Button.h"
#include "../../GraphicsEngine/DX/Direct2D.h"
#include <vector>
#include "TextBox.h"
#include "NotificationList.h"
//struct BitmapInfo
//{
//	ID2D1Bitmap* bitmap = nullptr;
//	std::string name = "";
//};
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
	bool AddNotification(Room *room, LogEvent *event);
	bool RemoveNotification(Room *room, Event::Type type);

	bool GetButtonOcclude();

	void ScrollActiveLog();
	void Update();
	void Draw();

private:
	
	
	void mUpdateButtons();

	std::vector<Button*> mButtonVector;
	std::vector<std::string> mButtonNames;
	

	std::vector<TextBox*> mTextBoxVector;
	std::vector<std::string> mTextBoxNames;

	NotificationList *mNotificationList;
	bool mNotificationListIsActive;

	bool mButtonOccludes;
};
