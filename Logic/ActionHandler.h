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

	struct Info
	{
		int type;
		float pos_x;
		float pos_z;
		int *ActionPtr;
		uint32_t data;
	};

	ActionHandler();
	~ActionHandler();

	virtual reality Init(Actions *pActions);
	virtual reality Update(ObserverInfo *pObsInf) override;
	virtual Info AddAction(float x, float z);
	virtual reality SwitchWaitingState();
	uint32_t *GetLastAction();

	void RotatePendingAction();
	const bool IsWaiting() const;
	ObserverInfo GetLastEvent() const;

	void InitFromFile(std::vector<Actions::Info> input, std::vector<int*> &output);

private:
	// Graphical actions.
	Actions *mpActions;


	ObserverInfo mLastEvent;
	ActionData mActionRotation = Rotation_0;
	bool mWaitingForClick;



	// Logical actions.
};