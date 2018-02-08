#include "Event.h"

std::string Event::GetString(Event::Type type)
{
	switch (type)
	{
		case Fire:
			return "Fire";
		case Water:
			return "Water";
		case Gas:
			return "Gas";
		default:
			return "unkownEventType";
	}
}

int Event::GetID(Event::Type type)
{
	switch (type)
	{
		case Fire:
			return 0;
		case Water:
			return 1;
		case Gas:
			return 2;
		default:
			return (-1);
	}
}
