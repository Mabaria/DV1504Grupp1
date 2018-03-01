#pragma once

#include "../../../UI/UIElements/Panel2D.h"
#include "../../../UI/UIElements/Panel3D.h"
#include "../../../Logic/Boat/Boat.h"
#include "../../../GraphicsEngine/Quad.h"
#include "../../../IO/Picking.h"
#include "../../../UI/GUI/EventMenu.h"
#include "../../../UI/UIElements/InfoPanel.h"
#include "../../../IO/Keyboard.h"

class System : public Observer<ObserverInfo>, public Observer<Button>
{
public:
	System();
	~System();
	void BuildGraphicalUserInterface(
		const std::wstring windowName, 
		const int windowWidth, 
		const int windowHeight);
	void Run();
	virtual void Update(ObserverInfo *pickedRoom);


private:
	void mUpdate();
	void mDraw();
	void mHandleInput();

	void Update(Button * attribute);

	void mUpdateHover(std::string name, int index, bool activate);
	void mUpdateRoomInfo();

	void mUpdateEvents(Room *room);

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

	EventLog mEventLog;
	Boat mBoat;
	Ray mRay;

	Room* mpLastClickedRoom;
};