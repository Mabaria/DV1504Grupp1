#include "ActionHandler.h"

ActionHandler::ActionHandler()
{
	this->mpActions = nullptr;
}

ActionHandler::~ActionHandler()
{
}

void ActionHandler::Init(Actions * pActions)
{
	this->mpActions = pActions;
}

void ActionHandler::Update(ObserverInfo * pObsInf)
{
	this->mLastEvent = *pObsInf;
	this->mWaitingForClick = true;
	this->NotifyObservers(this);
}

void ActionHandler::AddAction(float x, float z)
{
	if (this->mLastEvent.actionData != No_Action)
	{
		ActionData transformed_action_data = (ActionData)this->mLastEvent.actionData;
		// If Rotation_Stationary is NOT SET, rotate
		if ((this->mLastEvent.actionData & 64) == 0) {
			transformed_action_data = (ActionData)(this->mLastEvent.actionData + this->mActionRotation);
		}
		this->mpActions->AddAction(x, z, transformed_action_data);
	}
}

void ActionHandler::SwitchWaitingState()
{
	this->mWaitingForClick = !this->mWaitingForClick;
	if (!this->mWaitingForClick) // Reset rotation between events
		this->mActionRotation = Rotation_0;
}

void ActionHandler::RotatePendingAction()
{
	
	uint32_t temp = this->mActionRotation;
	temp = (temp + 16) % 64; // Rotation is ordered in increments of 16 from 0 to 48;
	this->mActionRotation = (ActionData)temp;
}

const bool ActionHandler::IsWaiting() const
{
	return this->mWaitingForClick;
}

ObserverInfo ActionHandler::GetLastEvent() const
{
	return this->mLastEvent;
}
