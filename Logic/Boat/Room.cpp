#include "Room.h"

Room::Room()
{
	this->mIndexInBoat = -1;
	this->mIndexInDeck = -1;
	this->mDeckIndex = -1;

	this->InitRoomInfo();
}

Room::Room(RoomDesc desc)
{
	this->InitFromDesc(desc);

	this->InitRoomInfo();
}

Room::~Room()
{

}



/**
*	Room specific
*/

void Room::SetIndex(int index)
{
	this->mIndexInBoat = index;
	this->mSensor.SetRoomIndex(this->mIndexInBoat);
}

void Room::SetName(std::string name)
{
	this->mName = name;
}

void Room::SetAABB(const AABB &boundingBox)
{
	this->mBoundingBox = boundingBox;
}

float Room::CheckRayCollision(const Ray & rRay)
{
	return Picking::IsRayIntersectingAABB(rRay, this->mBoundingBox);
}

std::string Room::GetName() const
{
	return this->mName;
}

void Room::InitFromDesc(RoomDesc desc)
{
	this->mName = desc.name;
	this->mIndexInBoat = desc.indexInBoat;
	this->mIndexInDeck = desc.indexInDeck;
	this->mDeckIndex = desc.deckIndex;
	this->mDeckName = desc.deckName;
	this->mSensor.SetRoomIndex(desc.indexInBoat);
	this->mSensor.SetEventLog(desc.pEventLog);
	this->mSensor.SetInputTypes(desc.inputTypes);
	this->mSensor.SetActiveEvent(desc.activeIndex, desc.pActiveEvent);
}

int Room::GetIndexInBoat() const
{
	return this->mIndexInBoat;
}

int Room::GetIndexInDeck() const
{
	return this->mIndexInDeck;
}




/**
*	Deck specific
*/

void Room::SetDeckName(std::string name)
{
	this->mDeckName = name;
}

std::string Room::GetDeckName() const
{
	return this->mDeckName;
}

int Room::GetDeckIndex() const
{
	return this->mDeckIndex;
}



/**
*	Log specific
*/

void Room::SetActiveEvent(int index, ActiveEvent *pActiveEvent)
{
	this->mSensor.SetActiveEvent(index, pActiveEvent);
}

void Room::SetEventLog(EventLog *pEventLog)
{
	this->mSensor.SetEventLog(pEventLog);
}



/**
*	Event specific
*/

bool Room::AddSensorEvent(Event::Type type)
{
	return this->mSensor.AutoTrigger(type);
}

bool Room::AddPlotterEvent(Event::Type type)
{
	return this->mSensor.PlotTrigger(type);
}

bool Room::ClearEvent(Event::Type type)
{
	return this->mSensor.ClearEvent(type);
}

void Room::AddInputType(Event::Type type)
{
	this->mSensor.AddInputType(type);
}

int Room::GetActiveEventIndex() const
{
	return this->mSensor.GetActiveEventIndex();
}

std::vector<LogEvent*> Room::GetActiveEvents() const
{
	return this->mSensor.GetActiveEvents();
}



/**
*	Disk specific
*/

std::string Room::WriteString() const
{
	std::string print = "";

	print += "r ";
	print += this->mName;
	print += " / ";
	print += this->mSensor.WriteString();

	return print;
}

void Room::InitRoomInfo()
{
	// Takes the mean distance from origin for each of the axes.
	this->mRoomInfo.centerPosition.x = 
		(this->mBoundingBox.x.max + this->mBoundingBox.x.min) / 2.0f;
	this->mRoomInfo.centerPosition.y = 
		(this->mBoundingBox.y.max + this->mBoundingBox.y.min) / 2.0f;
	this->mRoomInfo.centerPosition.z = 
		(this->mBoundingBox.z.max + this->mBoundingBox.z.min) / 2.0f;
	
	// Takes the difference between the max and min points for each
	// of the axes (unsigned).
	this->mRoomInfo.size.x = std::abs(
		this->mBoundingBox.x.max - this->mBoundingBox.x.min);
	this->mRoomInfo.size.y = std::abs(					   
		this->mBoundingBox.y.max - this->mBoundingBox.y.min);
	this->mRoomInfo.size.z = std::abs(					   
		this->mBoundingBox.z.max - this->mBoundingBox.z.min);
	
	// For distance to corner.
	float x_dist = this->mBoundingBox.x.max - this->mRoomInfo.centerPosition.x;
	float y_dist = this->mBoundingBox.y.max - this->mRoomInfo.centerPosition.y;
	float z_dist = this->mBoundingBox.z.max - this->mRoomInfo.centerPosition.z;
	
	// Vector length calculation.
	this->mRoomInfo.distanceToCorner = 
		sqrt(pow(x_dist, 2) + pow(y_dist, 2) + pow(z_dist, 2));
}

const RoomInfo Room::GetRoomInfo() const
{
	return this->mRoomInfo;
}

const XMFLOAT3 Room::GetRoomCenter() const
{
	return this->mRoomInfo.centerPosition;
}

const float Room::GetDistanceToCorner() const
{
	return this->mRoomInfo.distanceToCorner;
}

const XMFLOAT3 Room::GetRoomSize() const
{
	return this->mRoomInfo.size;
}
