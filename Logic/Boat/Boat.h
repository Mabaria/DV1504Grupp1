#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Deck.h"
#include "Room.h"

#include "../Eventlog/EventLog.h"
#include "../Event/Event.h"

#include "../../IO/Picking.h"
#include "../../UI/UIElements/MeshObject.h"

/**
*	---------------- HOW TO USE ----------------
*
*	Get started:
*		Simply call the LoadFromFile_Boat() with a filepath to the .boat file
*		you want to use. The log will be created automatically.
*
*		The log need to know what file to append all it's information to. For
*		this, you need to specify the paths to
*			* the .log that will be filled with events and actions,
*			* the log .meta that stores additional information about actions, and
*			* the room logs meta folder destination where all the room logs will
*			  store their own meta files.
*		It is important to know that the first two paths is specifying a file,
*		while the last one only specifies a path to a directory. The function you
*		have to call for this is (with example paths):
*			* Boat::SetLogPath("../../Savefiles/Logs/Log.log");
*			* Boat::SetLogMetaPath("../../Savefiles/Metafiles/Log.meta");
*			* Boat::SetRoomMetaDir("../../Savefiles/Metafiles/RoomLogMetas/");
*
*
*	Recover from an older session:
*		To recover you will need to specify
*			* .boat			- Includes the boat structure.
*			* .log			- Includes all the events and actions.
*			* .meta (log)	- Includes extra information about actions such as
*							  rotation and position.
*			* room logs meta folder
*							- This is the folder containing ALL metafiles for
*							  all room logs.
*							- Includes all the rooms currently active events and
*							  actions.
*		
*		Functions to call will be (in order):
*			1. LoadFromFile_Boat( .boat )
*			2. LoadFromFile_Log( .log , .meta , metafolder/ )
*
*
*
*	-- Room
*	
*	Get the room's index:
*		The room stores three different indices.
*			* Index_Boat		- The room's index in the boat.
*			* Index_Deck		- The room's index in the deck.
*			* Index_DeckInBoat	- The room's deck index in the boat.
*		You can get these indices by first obtaining the rooms pointer, then
*		call the corresponding GetIndex_[...]() function.
*
*
*
*	-- Events
*
*	Add an Event:
*		Get the pointer to the room you want to clear an Event in. The room will
*		have two functions you can use to add events:
*			* AddSensorEvent()	- Will check if the rooms' sensor can detect the
*								  given type of event.
*			* AddPlotterEvent()	- Will add the event regardless of the events
*								  type.
*		Event types can be found in 'Event.h'. The events' start time will begin
*		automatically the same time as you call the function. If you try to add
*		an Event type that is already active in the room, it won't get added.
*
*	Clear an Event:
*		Get the pointer to the room you want to add the event to. Call the
*		ClearEvent() function with the type of the Event you wan't do clear as
*		argument.
*
*	Get all currently active Events in a Room:
*		Get the pointer to the room. Create a std::vector<LogEvent*> object and
*		pass it (as a reference) to the room's GetActiveEvents() function. This
*		function will fill your std::vector with pointers to ALL the currently
*		active Events in that room.
*		If you only wan't to know how many Events that are active in the room,
*		call the room's GetEventCount() function instead.
*
*
*
*	-- Actions
*
*	Add an Action:
*		Get the pointer to the room you want to add the Action to. Create a
*		LogAction::Desc description and fill it with the information of the action.
*		You have to specify
*			* The type of the Action (LogAction::Type).
*			* The *int index to the graphic action. (Found in Actions.h)
*			* The world position of the action in x- and z-axis.
*			* The rotation settings of the action.
*			* 'start'  - New action or cleared action. If you create a NEW
*						 action for the system, set this as true.
*			* 'active' - If the action is still active. If you create a NEW
*					     action for the system, set this as true.
*		The description also includes 'roomName' and 'ID'. These two will be
*		filled automatically by the Logic system. When you have the description
*		ready to go, call the room's AddAction function and pass it as and
*		argument.
*
*	Clear and Action:
*		Get the pointer to the room you want to clear an Action in. Pass the *int
*		index from the graphical action object as an argument to the room's
*		ClearAction function.
*
*	Get all currently active Actions in a Room:
*		Get the pointer to the room. Create a std::vector<LogAction*> object and
*		pass it (as a reference) to the room's GetActiveActions() function. This
*		function will fill your std::vector with pointers to ALL the currently
*		active Actions in that room.
*		If you only wan't to know how many Actions that are active in the room,
*		call the room's GetActionCount() function instead.
*
*
*	-- Notations
*	
*	When clearing an Event/Action, the Log will automatically create a new Event/
*	Action set as inactive. This is just to log the clearing.
*
*	LogElement is only used to find the right order of added events and actions.
*/

class Boat
{
public:

	Boat();
	~Boat();
	
	// Boat specific
	std::string GetModelName() const;
	void SetModelName(std::string name);

	// Deck specific
	void AddDeck(std::string name);
	Deck* GetDeckPointer(std::string name);

	// Room specific
	bool AddRoom(std::string roomName,
		std::string deckName,
		int inputs);
	Room* GetRoomPointer(std::string name);
	Room* GetRoomPointerAt(int index_Boat);
	Room* GetPickedRoom(Ray ray);
	int GetRoomCount() const;

	// Same as GetPickedRoom() although it returns the position in which
	// the click hits as an XMFLOAT3 if it hits a room.
	// No hit returns {-100, -100, -100}.
	XMFLOAT3 GetPickedPosition(Ray ray);

	// Log specific
	int GetTotalEventCount() const;
	int GetTotalActionCount() const;

	int GetActiveEventCount() const;
	int GetActiveActionCount() const;

	// Disk specific
	void SetLogPath(std::string folderPath);
	void SetLogMetaPath(std::string folderPath);
	void SetRoomMetaDir(std::string folderPath);

	void ClearFiles() const;

	void SaveToFile_Boat(std::string filePath) const;
	bool LoadFromFile_Boat(std::string filePath);

	void SaveToFile_Log(std::string filePath, std::string metaFile, std::string roomLogsFolderPath) const;
	bool LoadFromFile_Log(std::string filePath, std::string metaFile, std::string roomLogsFolderPath);

	void SaveToFile_Log() const;
	bool LoadFromFile_Log();

	// Room data specific
	bool LoadBoundingBoxes(
		Mesh *meshList,
		DirectX::XMMATRIX **matrixList,
		int amount);

private:
	
	// Returns -1 if item is not found
	int GetRoomIndex_Boat(std::string roomName);
	int GetRoomIndex_Deck(std::string roomName);
	int GetDeckIndex(std::string deckName);

	std::string GetDeckNameByRoomIndex(int index);

	// Boat information
	std::string mModelName;

	// Structure
	std::vector<Deck*> mpDecks;

	// Bounding volumes
	std::vector<AABB> mBoundingAABB;
	std::vector<DirectX::XMMATRIX> mFloorMatrix;
	std::vector<DirectX::XMMATRIX> mInverseFloorMatrix;

	EventLog mEventLog;
};
