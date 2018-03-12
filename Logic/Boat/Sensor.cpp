#include "Sensor.h"

Sensor::Sensor()
{
	this->mInputTypes = 0;
}

Sensor::~Sensor()
{
}



/**
*	Sensor specific
*/

void Sensor::AddInputType(Event::Type type)
{
	this->mInputTypes |= type;
}

void Sensor::RemoveInputType(Event::Type type)
{
	this->mInputTypes &= ~type;
}

void Sensor::SetInputTypes(int types)
{
	this->mInputTypes = types;
}

void Sensor::GetInputTypes(std::vector<Event::Type> &output) const
{
	output.clear();

	for (int i = 0; i < Event::nrOfTypes; i++)
	{
		if (this->CanDetect(Event::GetType(i)))
			output.push_back(Event::GetType(i));
	}
}

int Sensor::GetInputTypes_Int() const
{
	return this->mInputTypes;
}

bool Sensor::CanDetect(Event::Type type) const
{
	return type & this->mInputTypes;
}
