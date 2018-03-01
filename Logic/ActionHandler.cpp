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
}

void ActionHandler::AddAction(float x, float z)
{
	this->mpActions->AddAction(x, z, this->mLastEvent.actionData);
}

void ActionHandler::SwitchWaitingState()
{
	this->mWaitingForClick = !this->mWaitingForClick;
}

const bool ActionHandler::IsWaiting() const
{
	return this->mWaitingForClick;
}
