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
	this->mWaitingForClick = false;
}
