#pragma once
#include "Button.h"
#include "TextBox.h"
#include "../../Logic/Timer.h"
#include "../../Logic/Event/Event.h"
#include "../../Logic/Boat/Room.h"

class NotificationObject
{
public:
	NotificationObject();
	~NotificationObject();

private:
	Timer timer;
	Room *room;
	//Event *event;


};
