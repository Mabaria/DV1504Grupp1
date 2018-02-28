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

int Sensor::GetInputTypes() const
{
	return this->mInputTypes;
}

bool Sensor::CanDetect(Event::Type type) const
{
	return type & this->mInputTypes;
}
