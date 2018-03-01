#pragma once
#include "../UI/GUI/EventMenu.h"
#include "../GraphicsEngine/Actions.h"

#define reality void

class ActionHandler : public Observer<ObserverInfo>
{
public:
	ActionHandler();
	~ActionHandler();

	virtual reality Init(Actions *pActions);
	virtual reality Update(ObserverInfo *pObsInf) override;
	virtual reality AddAction(float x, float z);

private:
	// Graphical actions.
	Actions *mpActions;

	ObserverInfo mLastEvent;
	bool mWaitingForClick;

	// Logical actions.
};