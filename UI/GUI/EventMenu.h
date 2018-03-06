#pragma once
#include "../../GraphicsEngine/Window.h"
#include "../../Logic/Eventlog/EventLog.h"
#include "../../Logic/Boat/Room.h"
#include "../../UI/UIElements/Panel2D.h"
#include "../../IO/ObserverPattern/Observer.h"
#include "../../Logic/ActionHandler.h"

class EventMenu : 
	public Observer<Button>,
	public Observer<ActionHandler>,
	public Observable<ObserverInfo> 
{
	enum ACTION_MODE {
		STANDARD, NUMBERS
	};
	enum INJURY_TYPE {
		MOVED, TREATED, REPORTED
	};
public:
	EventMenu();
	~EventMenu();

	bool Init(int parentWidth,
		int parentHeight,
		EventLog *pEventLog,
		LPCTSTR windowName,
		HWND *pWindow);
	bool OpenAt(Room *pRoom);
	bool Update();
	bool Draw() const;

	void Close();

	bool IsMouseInsidePanel();
	bool IsVisible();

	void SetActiveRoom(Room *room);

	virtual void Update(ActionHandler* attribute);
	virtual void Update(Button* attribute);
	Event::Type GetLastClicked();

private:
	int mMenuWidth, mMenuHeight;
	int mParentPanelWidth, mParentPanelHeight;
	bool mVisible, mButtonFocus, mDraggingWindow;
	int mDragX, mDragY;
	EventLog *mpEventLog;
	Panel2D *mpPanel;

	Room *mpActiveRoom;

	// For removing events.
	Event::Type mLastClicked;

	ACTION_MODE mActionMode;
	void mSwapActionMode();
	INJURY_TYPE mInjuryType;

	void mClearActionButtStates();


	void InitButtons();
};