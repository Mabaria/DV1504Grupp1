#pragma once
#include "Panel2D.h"

class CrewPanel : public Observer<Button>
{
public:
	CrewPanel();
	~CrewPanel();

	void Init(
		int width,
		int height,
		int top,
		int left,
		HWND parent,
		LPCTSTR title);
	void Update();
	virtual void Update(Button *button);
	void Draw();
	bool IsMouseInsidePanel();

private:
	Panel2D * mpPanel;

	bool mVisible, mButtonFocus, mDraggingWindow;
	int mDragX, mDragY;
	std::string *mSeaMen;

	void mCreateTextBoxesAndButtons();
	

};