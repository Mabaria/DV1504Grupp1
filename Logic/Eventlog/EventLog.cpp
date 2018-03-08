#include "EventLog.h"

EventLog::EventLog()
{
}

EventLog::~EventLog()
{
	for (int i = 0; i < (int)this->mpLogEvents.size(); i++)
	{
		delete this->mpLogEvents[i];
	}

	for (int i = 0; i < (int)this->mpActions.size(); i++)
	{
		delete this->mpActions[i];
	}
}



/**
*	Event specific
*/

LogEvent* EventLog::AddEvent(LogEvent::Desc desc)
{
	desc.ID = (int)this->mpLogEvents.size();
	this->mpLogEvents.push_back(new LogEvent(desc));

	this->AppendFiles_Event();

	return this->mpLogEvents.back();
}

int EventLog::GetTotalEventCount() const
{
	return (int)this->mpLogEvents.size();
}

int EventLog::GetActiveEventCount() const
{
	int count = 0;

	for (int i = 0; i < (int)this->mpLogEvents.size(); i++)
	{
		if (this->mpLogEvents[i]->IsActive())
			count++;
	}
	return count;
}

LogEvent* EventLog::GetEventPointerAt(int index)
{
	if (index < 0 || index >= (int)this->mpLogEvents.size())
		return nullptr;
	
	return this->mpLogEvents[index];
}

/**
*	Action specific
*/

LogAction* EventLog::AddAction(LogAction::Desc desc)
{
	desc.ID = (int)this->mpActions.size();
	this->mpActions.push_back(new LogAction(desc));

	this->AppendFiles_Action();

	return this->mpActions.back();
}

int EventLog::GetTotalActionCount() const
{
	return (int)this->mpActions.size();
}

int EventLog::GetActiveActionCount() const
{
	int count = 0;

	for (int i = 0; i < (int)this->mpActions.size(); i++)
	{
		if (this->mpActions[i]->IsActive())
			count++;
	}
	return count;
}

LogAction* EventLog::GetActionPointerAt(int index)
{
	if (index < 0 || index >= (int)this->mpActions.size())
		return nullptr;

	return this->mpActions[index];
}



/**
*	Log specific
*/

std::vector<LogEvent*> EventLog::GetActiveEvents() const
{
	std::vector<LogEvent*> list;

	for (int i = 0; i < (int)this->mpLogEvents.size(); i++)
	{
		if (this->mpLogEvents[i]->IsActive())
			list.push_back(this->mpLogEvents[i]);
	}

	return list;
}

std::vector<LogAction*> EventLog::GetActiveActions() const
{
	std::vector<LogAction*> list;

	for (int i = 0; i < (int)this->mpActions.size(); i++)
	{
		if (this->mpActions[i]->IsActive())
			list.push_back(this->mpActions[i]);
	}

	return list;
}

void EventLog::SetLogDir(std::string folderPath)
{
	this->mLogPath = folderPath;
}

void EventLog::SetMetaDir(std::string folderPath)
{
	this->mMetaPath = folderPath;
}



/**
*	Disk specific
*/

void EventLog::ClearFiles() const
{
	std::ofstream file_log;
	std::ofstream file_meta;

	// Open files in append mode
	file_log.open(this->mLogPath);
	file_meta.open(this->mMetaPath);

	file_log << "";
	file_meta << "";

	file_log.close();
	file_meta.close();
}

void EventLog::SaveToFile(std::string filePath, std::string metaFile) const
{
	std::ofstream file_log;
	std::ofstream file_meta;
	file_log.open(filePath);
	file_meta.open(metaFile);

	LogEvent *pEvent;
	LogAction *pAction;

	int nextEvent = 0;
	int nextAction = 0;

	while (
		nextEvent < (int)this->mpLogEvents.size() ||
		nextAction < (int)this->mpActions.size())
	{
		/**
		*	Find next line sorted by time
		*/

		// Both events and actions left to write
		if (
			nextEvent < (int)this->mpLogEvents.size() &&
			nextAction < (int)this->mpActions.size())
		{
			pEvent = this->mpLogEvents[nextEvent];
			pAction = this->mpActions[nextAction];

			if (pAction->GetTimestamp_Start() < pEvent->GetTimestamp_Start())
			{
				file_log
					<< "a"
					<< " "
					<< pAction->GetLogString()
					<< std::endl;
				file_meta
					<< pAction->GetMetaString()
					<< std::endl;
				nextAction++;
			}
			else
			{
				file_log
					<< "e "
					<< pEvent->GetFileString()
					<< std::endl;
				nextEvent++;
			}
		}

		// Only events left to write
		else if (nextEvent < (int)this->mpLogEvents.size())
		{
			pEvent = this->mpLogEvents[nextEvent];
			file_log
				<< "e "
				<< pEvent->GetFileString()
				<< std::endl;
			nextEvent++;
		}
		
		// Only actions left to write
		else
		{
			pAction = this->mpActions[nextAction];
			file_log
				<< "a"
				<< " "
				<< pAction->GetLogString()
				<< std::endl;
			file_meta
				<< pAction->GetMetaString()
				<< std::endl;
			nextAction++;
		}
	}

	file_log.close();
	file_meta.close();
}

bool EventLog::LoadFromFile(std::string filePath, std::string metaFile)
{
	std::ifstream file_log(filePath);
	std::ifstream file_meta(metaFile);

	std::string line_log;
	std::string line_meta;
	std::string word;

	int ID;

	if (file_log.is_open() && file_meta.is_open())
	{
		while (getline(file_log, line_log))
		{
			// Check first char in line
			switch (line_log[0])
			{
				case 'e':	// Event
					ID = this->GetTotalEventCount();
					this->mpLogEvents.push_back(new LogEvent(line_log, ID));
					break;
				case 'a':	// Action
					ID = this->GetTotalActionCount();
					getline(file_meta, line_meta);
					this->mpActions.push_back(new LogAction(line_log, line_meta));
					break;
			}
		}
	}
	else
		return false;
	
	return true;
}

void EventLog::AppendFiles_Event() const
{
	std::ofstream file;

	// Open file in append mode
	file.open(this->mLogPath, std::ios_base::app);

	LogEvent *pEvent = this->mpLogEvents.back();
	file
		<< "e "
		<< pEvent->GetFileString()
		<< std::endl;
}

void EventLog::AppendFiles_Action() const
{
	std::ofstream file_log;
	std::ofstream file_meta;

	// Open files in append mode
	file_log.open(this->mLogPath, std::ios_base::app);
	file_meta.open(this->mMetaPath, std::ios_base::app);

	LogAction *pAction = this->mpActions.back();
	file_log
		<< "a"
		<< " "
		<< pAction->GetLogString()
		<< std::endl;
	file_meta
		<< pAction->GetMetaString()
		<< std::endl;
}
