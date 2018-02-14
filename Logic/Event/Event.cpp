#include "Event.h"

std::string Event::GetString(Event::Type type)
{
	switch (type)
	{
		case Fire:
			return "Eldsv�da";
		case Injury:
			return "Personskada";
		case Water:
			return "Vattenl�cka";
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
