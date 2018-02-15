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
	bool Show(Room *pRoom, float screenX = 0.5f, float screenY = 0.5f);
	bool Update();
	bool Draw() const;
	virtual void Update(const Button& attribute);

private:
	int mMenuWidth, mMenuHeight;
	int mParentPanelWidth, mParentPanelHeight;
	bool mVisible;
	EventLog *mpEventLog;
	Panel2D *mpPanel;

	void InitButtons();
};