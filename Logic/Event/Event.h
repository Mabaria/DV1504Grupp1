#pragma once

#include <string>
#include <vector>


namespace Event
{
	 // All types of an event.
	 // Row-style allows an easy management of adding/ removing types to list
	 enum Type
	 {
	 	Fire,
	 	Water,
	 	Gas
	 };

	 std::string GetString(Type type);
	 int GetID(Type type);
}
