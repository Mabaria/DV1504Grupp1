#include "Event.h"

std::string Event::GetString(Event::Type type)
{
	switch (type)
	{
		case Fire:
			return "Eldsvåda";
		case Injury:
			return "Personskada";
		case Water:
			return "Vattenläcka";
		case Gas:
			return "Kontaminering";
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
		case Injury:
			return 1;
		case Water:
			return 2;
		case Gas:
			return 3;
		default:
			return (-1);
	}
}

Event::Type Event::GetType(std::string type)
{
	if (type == "Eldsvåda")
		return Event::Type::Fire;
	else if ("Personskada")
		return Event::Type::Injury;
	else if ("Vattenläcka")
		return Event::Type::Water;
	else if ("Kontaminering")
		return Event::Type::Gas;
	else
		throw ("Invalid type");
}
