#pragma once

#include "../../../UI/UIElements/Panel2D.h"
#include "../../../UI/UIElements/Panel3D.h"
#include "../../../Logic/Boat/Boat.h"
#include "../../../GraphicsEngine/Quad.h"
#include "../../../IO/Picking.h"
#include "../../../UI/GUI/EventMenu.h"
#include "../../../UI/UIElements/InfoPanel.h"
#include "../../../IO/Keyboard.h"
#include "../../../Logic/ActionHandler.h"
#include "../../../UI/UIElements/CrewPanel.h"

class System : public Observer<ObserverInfo>, public Observer<Button>, public Observable<std::string>
{
public:
	System();
	~System();
	void BuildGraphicalUserInterface(
		const std::wstring windowName, 
		const int windowWidth, 
		const int windowHeight);
	bool Run();
	virtual void Update(ObserverInfo *pickedRoom);

	int GetNrOfRooms() const;
	Room* GetRoomByIndex(int index);
	bool UpdateRoom(Room* room);

private:
	void mUpdate();
	void mDraw();
	void mHandleInput();

	void Update(Button * attribute);

	void mUpdateHover(std::string name, int index, bool activate);
	void mUpdateRoomInfo();

	// Select functions
	void mUnselect();
	void mSelectAndFocus(Room *picked_room);

	void mUpdateEvents(Room *room, bool automatic_input);

	void mUpdateGhostIcons();

	void mSetupPanels();
	void mSetupModels();
	void mSetupBoat();

	Panel2D *mpControlPanel;
	Panel2D *mpActiveLogPanel;
	EventMenu *mpMenuPanel;
			
	Panel3D *mpSideViewPanel;
	Panel3D *mpTopViewPanel;

	std::vector<Mesh*> mFloors;
	std::vector<Mesh*> mBounds;

	Camera *mpTopViewCameraPan;
	Camera *mpTopViewCameraRotate;
	Camera *mpSideViewCamera;
	Window *mpWindow;

	// User guide.
	InfoPanel mpInfoPanel;

	// Personnel Log
	CrewPanel mpCrewPanel;

	Log mEventLog;
	Boat mBoat;
	Ray mRay;

	Room* mpLastClickedRoom;

	ActionHandler mActionHandler;
};