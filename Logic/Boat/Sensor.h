#pragma once

#include "../Event/Event.h"

class Sensor
{
public:

	Sensor();
	~Sensor();

	// Sensor specific
	void AddInputType(Event::Type type);
	void RemoveInputType(Event::Type type);

	void SetInputTypes(int types);
	void GetInputTypes(std::vector<Event::Type> &output) const;
	int GetInputTypes_Int() const;

	bool CanDetect(Event::Type type) const;
	
private:

	int mInputTypes;
};
