#include "Log.h"

Log::Log()
{
}

Log::~Log()
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

LogEvent* Log::AddEvent(LogEvent::Desc desc)
{
	desc.ID = (int)this->mpLogEvents.size();
	this->mpLogEvents.push_back(new LogEvent(desc));

	this->AppendFiles_Event();

	this->mpElements.push_back(this->mpLogEvents.back());

	return this->mpLogEvents.back();
}

int Log::GetTotalEventCount() const
{
	return (int)this->mpLogEvents.size();
}

int Log::GetActiveEventCount() const
{
	int count = 0;

	for (int i = 0; i < (int)this->mpLogEvents.size(); i++)
	{
		if (this->mpLogEvents[i]->IsActive())
			count++;
	}
	return count;
}

LogEvent* Log::GetEventPointerAt(int index)
{
	if (index < 0 || index >= (int)this->mpLogEvents.size())
		return nullptr;
	
	return this->mpLogEvents[index];
}

/**
*	Action specific
*/

LogAction* Log::AddAction(LogAction::Desc desc)
{
	desc.ID = (int)this->mpActions.size();
	this->mpActions.push_back(new LogAction(desc));

	this->AppendFiles_Action();

	this->mpElements.push_back(this->mpActions.back());

	return this->mpActions.back();
}

int Log::GetTotalActionCount() const
{
	return (int)this->mpActions.size();
}

int Log::GetActiveActionCount() const
{
	int count = 0;

	for (int i = 0; i < (int)this->mpActions.size(); i++)
	{
		if (this->mpActions[i]->IsActive())
			count++;
	}
	return count;
}

LogAction* Log::GetActionPointerAt(int index)
{
	if (index < 0 || index >= (int)this->mpActions.size())
		return nullptr;

	return this->mpActions[index];
}



/**
*	Log specific
*/

std::vector<LogEvent*> Log::GetActiveEvents() const
{
	std::vector<LogEvent*> list;

	for (int i = 0; i < (int)this->mpLogEvents.size(); i++)
	{
		if (this->mpLogEvents[i]->IsActive())
			list.push_back(this->mpLogEvents[i]);
	}

	return list;
}

std::vector<LogAction*> Log::GetActiveActions() const
{
	std::vector<LogAction*> list;

	for (int i = 0; i < (int)this->mpActions.size(); i++)
	{
		if (this->mpActions[i]->IsActive())
			list.push_back(this->mpActions[i]);
	}

	return list;
}

void Log::SetLogDir(std::string folderPath)
{
	this->mLogPath = folderPath;
}

void Log::SetMetaDir(std::string folderPath)
{
	this->mMetaPath = folderPath;
}



/**
*	Disk specific
*/

void Log::ClearFiles() const
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

void Log::SaveToFile(std::string filePath, std::string metaFile) const
{
	std::ofstream file_log;
	std::ofstream file_meta;
	file_log.open(filePath);
	file_meta.open(metaFile);

	LogEvent *pEvent;
	LogAction *pAction;

	int size = this->mpElements.size();

	for (int i = 0; i < size; i++)
	{
		pEvent = nullptr;
		pAction = nullptr;

		pEvent = dynamic_cast<LogEvent*>(this->mpElements[i]);

		if (pEvent != nullptr)
		{
			file_log
				<< "e "
				<< pEvent->GetFileString()
				<< std::endl;
		}
		else
		{
			pAction = dynamic_cast<LogAction*>(this->mpElements[i]);

			file_log
				<< "a"
				<< " "
				<< pAction->GetLogString()
				<< std::endl;
			file_meta
				<< pAction->GetMetaString()
				<< std::endl;
		}
	}

	file_log.close();
	file_meta.close();
}

bool Log::LoadFromFile()
{
	return this->LoadFromFile(this->mLogPath, this->mMetaPath);
}

bool Log::LoadFromFile(std::string filePath, std::string metaFile)
{
	std::ifstream file_log(filePath);
	std::ifstream file_meta(metaFile);

	std::string line_log;
	std::string line_meta;
	std::string word;

	int ID;

	// It's ok to have an OR here, because the meta file
	// will never be used if there's no actions added. If
	// there's actions in the log, a meta file exists.
	if (file_log.is_open() || file_meta.is_open())
	{
		while (getline(file_log, line_log))
		{
			// Check first char in line
			switch (line_log[0])
			{
				case 'e':	// Event
					ID = this->GetTotalEventCount();
					this->mpLogEvents.push_back(new LogEvent(line_log, ID));

					this->mpElements.push_back(this->mpLogEvents.back());
					break;
				case 'a':	// Action
					ID = this->GetTotalActionCount();
					getline(file_meta, line_meta);
					this->mpActions.push_back(new LogAction(line_log, line_meta));

					this->mpElements.push_back(this->mpActions.back());
					break;
			}
		}
	}
	else
		return false;
	
	return true;
}

void Log::GetAllActiveActions(std::vector<Log::ActionInfo> &output)
{
	output.clear();
	for (int i = 0; i < (int)this->mpActions.size(); i++)
	{
		if (this->mpActions[i]->IsActive())
		{
			Log::ActionInfo info;
			info.index = i;
			info.pIndexPtr = nullptr;
			info.pAction = this->mpActions[i];
			output.push_back(info);
		}
	}
}

void Log::UpdateActionPointers(std::vector<Log::ActionInfo> &pointers)
{
	for (int i = 0; i < (int)pointers.size(); i++)
		this->mpActions[pointers[i].index]->SetActionIndex(pointers[i].pIndexPtr);
}

void Log::AppendFiles_Event() const
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

void Log::AppendFiles_Action() const
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
