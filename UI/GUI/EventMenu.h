#pragma once
#include "../../GraphicsEngine/Window.h"
#include "../../Logic/Eventlog/EventLog.h"
#include "../../Logic/Boat/Room.h"
#include "../../UI/UIElements/Panel2D.h"
#include "../../IO/ObserverPattern/Observer.h"
class EventMenu : public Observer<Button>, public Observable<Room>
{
public:
	EventMenu();
	~EventMenu();

	bool Init(float parentWidth,
		float parentHeight,
		EventLog *pEventLog,
		LPCTSTR windowName,
		HWND *pWindow);
	bool OpenAt(Room *pRoom);
	bool Update();
	bool Draw() const;

	bool IsMouseInsidePanel();
	bool IsVisible();

	virtual void Update( Button* attribute);

private:
	int mMenuWidth, mMenuHeight;
	int mParentPanelWidth, mParentPanelHeight;
	bool mVisible;
	EventLog *mpEventLog;
	Panel2D *mpPanel;

	Room *mpActiveRoom;

	void InitButtons();
};