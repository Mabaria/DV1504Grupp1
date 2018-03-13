#include "RoomLog.h"

RoomLog::RoomLog()
{
	mpEventLog = nullptr;
}

RoomLog::~RoomLog()
{
	
}



/**
*	Event specific
*/

bool RoomLog::AddEvent(Event::Type type)
{
	for (int i = 0; i < (int)this->mpEvents.size(); i++)
	{
		// Check if type is already active in room
		if (this->mpEvents[i]->GetType() == type)
			return false;
	}
	LogEvent::Desc desc;
	desc.type = type;
	desc.roomName = this->mRoomName;
	desc.start = true; // New event will be a 'start' event
	desc.active = true;

	LogEvent *pNewEvent = this->mpEventLog->AddEvent(desc);
	this->mpEvents.push_back(pNewEvent);

	// Update meta file
	this->UpdateMeta();

	return true;
}

bool RoomLog::ClearEvent(Event::Type type)
{
	for (int i = 0; i < (int)this->mpEvents.size(); i++)
	{
		// Find event with requested type
		if (this->mpEvents[i]->GetType() == type)
		{
			this->mpEvents[i]->SetInactive();

			// Create new event
			LogEvent::Desc desc;
			desc.type = type;
			desc.roomName = this->mpEvents[i]->GetRoomName();
			desc.start = false; // New event will be a 'stopped' event
			desc.active = false;

			this->mpEventLog->AddEvent(desc);

			this->mpEvents.erase(this->mpEvents.begin() + i);
			return true;
		}
	}

	// Update meta file
	this->UpdateMeta();

	return false;
}

int RoomLog::GetEventCount() const
{
	return (int)this->mpEvents.size();
}

void RoomLog::GetActiveEvents(std::vector<LogEvent*> &output)
{
	output.clear();

	for (int i = 0; i < (int)this->mpEvents.size(); i++)
		output.push_back(this->mpEvents[i]);
}



/**
*	Action specific
*/
 
bool RoomLog::AddAction(LogAction::Desc desc)
{
	desc.roomName = this->mRoomName;
	LogAction *pNewAction = this->mpEventLog->AddAction(desc);

	this->mpActions.push_back(pNewAction);

	// Update meta file
	this->UpdateMeta();

	return true;
}

bool RoomLog::ClearAction(int *actionIndex)
{
	for (int i = 0; i < (int)this->mpActions.size(); i++)
	{
		if (this->mpActions[i]->GetActionIndex() == actionIndex)
		{
			LogAction *pAction = this->mpActions[i];

			pAction->SetInactive();

			LogAction::Desc desc;
			desc.type = pAction->GetType();
			desc.roomName = pAction->GetRoomName();
			desc.pActionIndex = pAction->GetActionIndex();
			desc.pos_x = pAction->GetPos_X();
			desc.pos_z = pAction->GetPos_Z();
			desc.data = pAction->GetData();
			desc.start = false;
			desc.active = false;

			this->mpEventLog->AddAction(desc);

			this->mpActions.erase(this->mpActions.begin() + i);

			// Update meta file
			this->UpdateMeta();

			return true;
		}
	}

	return false;
}

int RoomLog::GetActionCount() const
{
	return (int)this->mpActions.size();
}

void RoomLog::GetActiveActions(std::vector<LogAction*> &output)
{
	output.clear();
	for (int i = 0; i < (int)this->mpActions.size(); i++)
		output.push_back(this->mpActions[i]);
}



/**
*	Room specific
*/

void RoomLog::SetRoomName(std::string name)
{
	this->mRoomName = name;
}



/**
*	Log specific
*/

void RoomLog::SetEventLogPtr(Log *pEventLog)
{
	this->mpEventLog = pEventLog;
}



/**
*	Disk specific
*/


void RoomLog::SetMetaPath(std::string path)
{
	this->mMetaPath = path;
}

void RoomLog::ClearMeta() const
{
	std::ofstream file;
	file.open(this->mMetaPath + this->mRoomName + ".meta");

	file << "";

	file.close();
}

void RoomLog::SaveToFile(std::string folderPath) const
{
	std::string filePath = this->RealPath(folderPath);
	
	std::ofstream file;
	file.open(filePath);

	for (int i = 0; i < (int)this->mpEvents.size(); i++)
		file << "e " << this->mpEvents[i]->GetID() << std::endl;

	for (int i = 0; i < (int)this->mpActions.size(); i++)
		file << "a " << this->mpActions[i]->GetID() << std::endl;

	file.close();
}

bool RoomLog::LoadFromFile(std::string folderPath)
{
	std::string filePath = this->RealPath(folderPath);

	std::ifstream file(filePath);
	std::string line;

	std::string scrap;
	int number;

	std::stringstream ss;
	
	if (file.is_open())
	{
		while (getline(file, line))
		{
			ss.clear();
			ss.str(line);

			// Check first character of line
			switch (line[0])
			{
				case 'e':	// Event
					ss >> scrap;
					ss >> number;
					this->mpEvents.push_back(this->mpEventLog->GetEventPointerAt(number));
					this->mpEvents.back()->SetActive();
					break;

				case 'a':	// Action
					//ss >> scrap >> number;
					ss >> scrap;
					ss >> number;
					this->mpActions.push_back(this->mpEventLog->GetActionPointerAt(number));
					this->mpActions.back()->SetActive();
					break;

				default:	// Comments
					break;
			}
		}
		file.close();
	}
	else
	{
		return false;
	}

	return true;
}

void RoomLog::SaveToFile() const
{
	this->SaveToFile(this->mMetaPath);
}

bool RoomLog::LoadFromFile()
{
	return this->LoadFromFile(this->mMetaPath);
}

std::string RoomLog::RealPath(std::string folderPath) const
{
	if (folderPath[folderPath.size()-1] == '/')
		return folderPath + this->mRoomName + ".meta";
	else
		return folderPath + "/" + this->mRoomName + ".meta";
}

void RoomLog::UpdateMeta() const
{
	std::ofstream file;
	file.open(this->mMetaPath + this->mRoomName + ".meta");

	int eventSize = (int)this->mpEvents.size();
	int actionSize = (int)this->mpActions.size();

	if (eventSize == 0 && actionSize == 0)
	{
		file << "";
	}
	else
	{
		for (int i = 0; i < eventSize; i++)
			file << "e " << this->mpEvents[i]->GetID() << std::endl;

		for (int i = 0; i < actionSize; i++)
			file << "a " << this->mpActions[i]->GetID() << std::endl;
	}


	file.close();
}
