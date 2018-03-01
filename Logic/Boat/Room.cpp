#include "Room.h"

Room::Room()
{
	this->mIndex_Boat = -1;
	this->mIndex_Deck = -1;
	this->mIndex_DeckInBoat = -1;
}

Room::Room(Room::Room::Desc desc)
{
	this->InitFromDesc(desc);
}

Room::~Room()
{

}



/**
*	Room specific
*/

void Room::SetIndex_Boat(int index)
{
	this->mIndex_Boat = index;
}

void Room::SetIndex_Deck(int index)
{
	this->mIndex_Deck = index;
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

float Room::CheckWorldRayCollision(const Ray & rRay)
{
	return Picking::IsRayIntersectingAABB(rRay, this->mWorldBoundingBox);
}

std::string Room::GetName() const
{
	return this->mName;
}

void Room::InitFromDesc(Room::Desc desc)
{
	this->mSensor.SetInputTypes(desc.inputs);
	this->mIndex_Boat = desc.index_Boat;
	this->mIndex_Deck = desc.index_Deck;
	this->mIndex_DeckInBoat = desc.index_DeckInBoat;
	this->mName = desc.name;
	this->mDeckName = desc.deckName;
	this->mRoomLog.SetEventLogPtr(desc.pEventLog);
	this->mRoomLog.SetRoomName(desc.name);
}

int Room::GetIndexInBoat() const
{
	return this->mIndex_Boat;
}

int Room::GetIndexInDeck() const
{
	return this->mIndex_Deck;
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

int Room::GetIndex_Boat() const
{
	return this->mIndex_Boat;
}

int Room::GetIndex_Deck() const
{
	return this->mIndex_Deck;
}

int Room::GetIndex_DeckInBoat() const
{
	return this->mIndex_DeckInBoat;
}



/**
*	Event specific
*/

bool Room::AddSensorEvent(Event::Type type)
{
	if (this->mSensor.CanDetect(type))
	{
		return this->mRoomLog.AddEvent(type);
	}

	// If sensor can't detect given type
	return false;
}

bool Room::AddPlotterEvent(Event::Type type)
{
	return this->mRoomLog.AddEvent(type);
}

bool Room::ClearEvent(Event::Type type)
{
	return this->mRoomLog.ClearEvent(type);
}

void Room::AddInputType(Event::Type type)
{
	this->mSensor.AddInputType(type);
}

std::vector<LogEvent*> Room::GetActiveEvents()
{
	return this->mRoomLog.GetActiveEvents();
}

int Room::GetEventCount() const
{
	return this->mRoomLog.GetEventCount();
}



/**
*	Action specific
*/

bool Room::AddAction(LogAction::Desc desc)
{
	return this->mRoomLog.AddAction(desc);
}

bool Room::ClearAction(int actionIndex)
{
	return this->mRoomLog.ClearAction(actionIndex);
}

int Room::GetActionCount() const
{
	return this->mRoomLog.GetActionCount();
}



/**
*	Disk specific
*/

std::string Room::GetString() const
{
	std::stringstream ss;

	ss << "r ";
	ss << this->mName;
	ss << " / ";
	ss << this->mSensor.GetInputTypes_Int();

	return ss.str();
}

Room::Desc Room::FillRoomDescFromLine(std::string line)
{
	Room::Desc desc;

	std::stringstream buffer;
	std::string roomName, word;

	buffer.str(line);

	/**
	*	Get room name
	*/
	roomName = "";
	buffer >> word; // Get 'r'
	buffer >> word;

	while (word != "/")
	{
		if (roomName != "")
		roomName += " ";
		roomName +=	word;

		buffer >> word;
	}
	desc.name = Room::CorrectName(roomName);

	/**
	*	Get sensor data
	*/
	buffer >> desc.inputs;	// Input types (stored as int)
	
	return desc;
}

std::string Room::CorrectName(std::string name)
{
	std::string newName = "";

	for (int i = 0; i < (int)name.size(); i++)
	{
		int c = name[i];
		
		switch (name[i])
		{
		case -91: // ¥
			newName += "å";
			break;
		case -92: // ¤
			newName += "ä";
			break;
		case -74: // ¶
			newName += "ö";
			break;

		case -61: // Character skip
			break;

		default:
			newName += name[i];
			break;
		}
	}
	return newName;
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
 
	// Saving this bounding box for picking purposes.
	this->mWorldBoundingBox = bounding_box;

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
