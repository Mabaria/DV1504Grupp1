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
	virtual reality SwitchWaitingState();

	void RotatePendingAction();
	const bool IsWaiting() const;
private:
	// Graphical actions.
	Actions *mpActions;


	ObserverInfo mLastEvent;
	ActionData mActionRotation = Rotation_0;
	bool mWaitingForClick;

	// Logical actions.
};