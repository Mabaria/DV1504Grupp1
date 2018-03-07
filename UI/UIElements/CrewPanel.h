#pragma once
#include "Panel2D.h"
#include "../../Logic/Timer.h"

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
	struct TimeData {
		time_t startTime[22];
		char textState[22];
	};
	//Timer *mTimers[22];
	TimeData mTimeData;
	Panel2D * mpPanel;
	Timer mtimer;
	bool mVisible, mButtonFocus, mDraggingWindow;
	int mDragX, mDragY;
	std::string *mSeaMen;
	int mFontSize;

	void mCreateTextBoxesAndButtons();
	void mOpenWindow();
	
	void mInitTimers();
	void mSaveToDisk();
};