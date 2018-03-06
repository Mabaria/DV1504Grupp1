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
	if (this->mLastEvent.actionData != No_Action)
	{
		this->mpActions->AddAction(x, z, this->mLastEvent.actionData);
	}
}

void ActionHandler::SwitchWaitingState()
{
	this->mWaitingForClick = !this->mWaitingForClick;
}

uint32_t * ActionHandler::GetLastAction()
{
	return &this->mLastEvent.actionData;
}

const bool ActionHandler::IsWaiting() const
{
	return this->mWaitingForClick;
}
