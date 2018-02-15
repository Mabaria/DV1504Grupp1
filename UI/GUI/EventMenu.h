#pragma once
#include "../../GraphicsEngine/Window.h"
#include "../../Logic/Eventlog/EventLog.h"
#include "../../Logic/Boat/Room.h"
#include "../../UI/UIElements/Panel2D.h"
#include "../../IO/ObserverPattern/Observer.h"
class EventMenu : public Observer<Button>
{
public:
	EventMenu();
	~EventMenu();

	bool Init(float parentWidth,
		float parentHeight,
		EventLog *pEventLog,
		LPCTSTR windowName,
		Window *pWindow);
	bool OpenAt(Room *pRoom, float panelX = 0.5f, float panelY = 0.5f);
	bool Update();
	bool Draw() const;
	virtual void Update(const Button* attribute);

private:
	int mMenuWidth, mMenuHeight;
	int mParentPanelWidth, mParentPanelHeight;
	bool mVisible;
	EventLog *mpEventLog;
	Panel2D *mpPanel;

	Room *mpActiveRoom;

	void InitButtons();
};