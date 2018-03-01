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



/**
*	Action specific
*/

//bool Room::AddAction(LogAction::Type type, GraphicalAction *pAction)
//{
//	return this->mRoomLog.AddAction(type, pAction);
//}

// FIXME when Graphical actions exists
//bool ClearAction(GraphicalAction *pAction)
//{
//	return this->mRoomLog.ClearAction(pAction);
//}



/**
*	Disk specific
*/

std::string Room::GetString() const
{
	std::stringstream ss;

	ss << "r ";
	ss << this->mName;
	ss << " / ";
	ss << this->mSensor.GetInputTypes();

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
