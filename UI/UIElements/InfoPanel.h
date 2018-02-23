#pragma once
#include "Panel2D.h"

class InfoPanel : public Observer<Button>
{
public:
	InfoPanel();
	~InfoPanel();

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
	int mParentPanelWidth, mParentPanelHeight;
	bool mVisible, mButtonFocus, mDraggingWindow;
	int mDragX, mDragY;

	Panel2D *mpPanel;

};