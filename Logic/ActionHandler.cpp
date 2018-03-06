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
		this->mpActions->AddAction(x, z, this->mLastEvent.actionData);
		this->NotifyObservers(this);
	}
}

void ActionHandler::SwitchWaitingState()
{
	this->mWaitingForClick = !this->mWaitingForClick;
	this->NotifyObservers(this);
}

const bool ActionHandler::IsWaiting() const
{
	return this->mWaitingForClick;
}

ObserverInfo ActionHandler::GetLastEvent() const
{
	return this->mLastEvent;
}
