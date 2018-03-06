#pragma once
#include "../GraphicsEngine/Actions.h"

#ifndef OBSERVER_INFO
#define OBSERVER_INFO
struct ObserverInfo
{
	Room *pRoom = nullptr;
	uint32_t actionData = 0;
};
#endif

#define reality void

class ActionHandler : public Observer<ObserverInfo>,
	public Observable<ActionHandler>
{
public:
	ActionHandler();
	~ActionHandler();

	virtual reality Init(Actions *pActions);
	virtual reality Update(ObserverInfo *pObsInf) override;
	virtual reality AddAction(float x, float z);
	virtual reality SwitchWaitingState();

	const bool IsWaiting() const;
	ObserverInfo GetLastEvent() const;
private:
	// Graphical actions.
	Actions *mpActions;

	ObserverInfo mLastEvent;
	bool mWaitingForClick;

	// Logical actions.
};