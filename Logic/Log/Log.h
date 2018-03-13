#pragma once

#include <vector>
#include <fstream>

#include "LogElement.h"
#include "LogEvent.h"
#include "LogAction.h"

/**
*	The log will be able to store/load content to/from a file
*	In the file, events are listed in rows. One row = one event
*
*	Example:
* 23-01-2018 14:29:21	Fire	|	started		|		Room4
* 23-01-2018 14:31:32	Water	|	stopped		|		Room1
*	
*/

class Log
{
public:

	struct Desc_Action
	{
		LogAction::Type type;
		std::string roomName;
		int actionIndex;
		float pos_x;
		float pos_y;
	};

	struct ActionInfo
	{
		int index;
		int *pIndexPtr;
		LogAction *pAction;
	};

	Log();
	~Log();

	// Event Specific
	LogEvent* AddEvent(LogEvent::Desc desc);
															/* Pushing an event
															at the end of the list,
															returns index of event in
															the active log */
	//bool ClearEvent(LogEvent* pEvent);
	int GetTotalEventCount() const;
	int GetActiveEventCount() const;
	LogEvent* GetEventPointerAt(int index);

	// Action specific
	LogAction* AddAction(LogAction::Desc desc);
	int GetTotalActionCount() const;
	int GetActiveActionCount() const;
	LogAction* GetActionPointerAt(int index);

	// Log specific
	std::vector<LogEvent*> GetActiveEvents() const;
	std::vector<LogAction*> GetActiveActions() const;
	
	// Disk specific
	void SetLogDir(std::string folderPath);
	void SetMetaDir(std::string folderPath);

	void ClearFiles() const;

	void SaveToFile(std::string filePath, std::string metaFile) const;
	bool LoadFromFile();
	bool LoadFromFile(std::string filePath, std::string metaFile);

	void GetAllActiveActions(std::vector<ActionInfo> &output);
	void UpdateActionPointers(std::vector<Log::ActionInfo> &pointers);

private:

	void AppendFiles_Event() const;
	void AppendFiles_Action() const;

	std::vector<LogEvent*> mpLogEvents;
	std::vector<LogAction*> mpActions;
	std::vector<LogElement*> mpElements;

	std::string mLogPath;
	std::string mMetaPath;
};
