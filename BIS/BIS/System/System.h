#pragma once

#include "../../../UI/UIElements/Panel2D.h"
#include "../../../UI/UIElements/Panel3D.h"
#include "../../../Logic/Boat/Boat.h"
#include "../../../GraphicsEngine/Quad.h"
#include "../../../IO/Picking.h"
#include "../../../UI/GUI/EventMenu.h"
#include "../../../UI/UIElements/InfoPanel.h"

class System : public Observer<Room>
{
public:
	System();
	~System();
	void BuildGraphicalUserInterface(
		const std::wstring windowName, 
		const int windowWidth, 
		const int windowHeight);
	void Run();
	virtual void Update( Room *pickedRoom);


private:
	void mUpdate();
	void mDraw();
	void mHandleInput();

	void mUpdateEvents(Room *room);

	void mSetupPanels();
	void mSetupModels();
	void mSetupBoat();
	
	void mSetupInfoWindow(
		int windowWidth, 
		int windowHeight, 
		std::wstring windowName);

	Panel2D *mpControlPanel;
	Panel2D *mpActiveLogPanel;
	EventMenu *mpMenuPanel;
			
	Panel3D *mpSideViewPanel;
	Panel3D *mpTopViewPanel;

	std::vector<Mesh*> mFloors;
	std::vector<Mesh*> mBounds;
	std::vector<Quad*> mTexts;

	Camera *mpTopViewCamera;
	Camera *mpSideViewCamera;
	Window *mpWindow;

	// User guide.
	//Panel2D *mpInfoPanel;
	InfoPanel mpInfoPanel;

	EventLog mEventLog;
	Boat mBoat;
	Ray mRay;
};