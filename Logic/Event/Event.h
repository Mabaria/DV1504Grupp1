#pragma once

#include <string>
#include <vector>
#include <cstdint>

#define Mask_Fire 1
#define Mask_Injury 2
#define Mask_Water 4
#define Mask_Gas 8

namespace Event
{
	// All types of an event.
	// Row-style allows an easy management of adding/ removing types to list
	const int nrOfTypes = 4;

	enum Type
	{
		Fire = 1,
		Injury = 2,
		Water = 4,
		Gas = 8
	};

	std::string GetString(Type type);
	int GetID(Type type);
	Event::Type GetType(std::string type);
	Event::Type GetType(int id);
}
