#include "Event.h"

std::string Event::GetString(Event::Type type)
{
	switch (type)
	{
		case Fire:
			return "Eldsvåda";
		case Water:
			return "Vatteninträngning";
		case Gas:
			return "Kontaminering";
		case Injury:
			return "Personskada";
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
		case Injury:
			return 3;
		default:
			return (-1);
	}
}
