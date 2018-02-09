#pragma once
#include "Button.h"
#include "TextBox.h"

class NotificationObject
{
public:
	NotificationObject();
	~NotificationObject();

private:
	std::vector<Button*> mButtonVector;
	std::vector<std::string> mButtonNames;


	void AddButton(int width,
		int height,
		int top,
		int left,
		std::string imageFilePath,
		std::string buttonName);

	void AddTextbox(
		int width,
		int height,
		int top,
		int left,
		std::string text,
		std::string name);

};
