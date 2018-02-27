#pragma once

#include <string>
#include <vector>

namespace Event
{
	// All types of an event.
	// Row-style allows an easy management of adding/ removing types to list
	const int nrOfTypes = 4;
	enum Type
	{
		Fire,
		Injury,
		Water,
		Gas
	};

	std::string GetString(Type type);
	int GetID(Type type);
	Event::Type GetType(std::string type);
}
