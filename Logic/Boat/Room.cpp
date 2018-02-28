#include "Room.h"

Room::Room()
{
	this->mIndexInBoat = -1;
	this->mIndexInDeck = -1;
	this->mDeckIndex = -1;
}

Room::Room(RoomDesc desc)
{
	this->InitFromDesc(desc);
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
*	Sensor specific
*/

std::vector<Event::Type> Room::GetInputTypes() const
{
	return this->mSensor.GetInputTypes();
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

void Room::InitRoomData(XMMATRIX matrix)
{
	// Transforming the AABB with the deck matrix.
	XMVECTOR min = {
		this->mBoundingBox.x.min,
		this->mBoundingBox.y.min,
		this->mBoundingBox.z.min
	};
	XMVECTOR max = {
		this->mBoundingBox.x.max,
		this->mBoundingBox.y.max,
		this->mBoundingBox.z.max
	};
	min = XMVector3Transform(min, matrix);
	max = XMVector3Transform(max, matrix);

	// Creating a new bounding box with transformed extrema.
	AABB bounding_box;
	bounding_box.x.min = XMVectorGetX(min);
	bounding_box.y.min = XMVectorGetY(min);
	bounding_box.z.min = XMVectorGetZ(min);
	bounding_box.x.max = XMVectorGetX(max);
	bounding_box.y.max = XMVectorGetY(max);
	bounding_box.z.max = XMVectorGetZ(max);
 
	// Takes the mean distance from origin for each of the axes.
	this->mRoomData.centerPosition.x = 
		(bounding_box.x.max + bounding_box.x.min) / 2.0f;
	this->mRoomData.centerPosition.y = 
		(bounding_box.y.max + bounding_box.y.min) / 2.0f;
	this->mRoomData.centerPosition.z = 
		(bounding_box.z.max + bounding_box.z.min) / 2.0f;

	// Takes the difference between the max and min points for each
	// of the axes (unsigned).
	this->mRoomData.size.x = std::abs(
		bounding_box.x.max - bounding_box.x.min);
	this->mRoomData.size.y = std::abs(
		bounding_box.y.max - bounding_box.y.min);
	this->mRoomData.size.z = std::abs(
		bounding_box.z.max - bounding_box.z.min);
	
	// For distance to corner.
	float x_dist = bounding_box.x.max - this->mRoomData.centerPosition.x;
	float y_dist = bounding_box.y.max - this->mRoomData.centerPosition.y;
	float z_dist = bounding_box.z.max - this->mRoomData.centerPosition.z;
	
	// Vector length calculation.
	this->mRoomData.distanceToCorner = 
		sqrt(pow(x_dist, 2) + pow(y_dist, 2) + pow(z_dist, 2));
}

const RoomData Room::GetRoomData() const
{
	return this->mRoomData;
}

const XMFLOAT3 Room::GetRoomCenter() const
{
	return this->mRoomData.centerPosition;
}

const float Room::GetDistanceToCorner() const
{
	return this->mRoomData.distanceToCorner;
}

const XMFLOAT3 Room::GetRoomSize() const
{
	return this->mRoomData.size;
}
