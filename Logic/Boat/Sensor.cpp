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

std::vector<Event::Type> Sensor::GetInputTypes() const
{
	std::vector<Event::Type> types;

	for (int i = 0; i < Event::nrOfTypes; i++)
	{
		if (this->CanDetect(Event::GetType(i)))
			types.push_back(Event::GetType(i));
	}

	return types;
}

int Sensor::GetInputTypes_Int() const
{
	return this->mInputTypes;
}

bool Sensor::CanDetect(Event::Type type) const
{
	return type & this->mInputTypes;
}
