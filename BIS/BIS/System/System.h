#pragma once

#include "../../../UI/UIElements/Panel2D.h"
#include "../../../UI/UIElements/Panel3D.h"
#include "../../../Logic/Boat/Boat.h"
#include "../../../GraphicsEngine/Quad.h"
#include "../../../IO/Picking.h"
#include "../../../UI/GUI/EventMenu.h"

/* TODO:
*Picking
*Kontrollknappar
*Båtgrejer
*/




class System : public Observer<Room>
{
public:
	System();
	~System();
	void BuildGraphicalUserInterface(const std::wstring windowName, const int windowWidth, const int windowHeight);
	void Run();
	virtual void Update( Room *pickedRoom);


private:
	void mUpdate();
	void mDraw();
	void mHandleInput();

	void mAddEvent(Room *room);
	void mRemoveEvent(Room *room);

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
	std::vector<Quad*> mTexts;

	Camera *mpTopViewCamera;
	Camera *mpSideViewCamera;
	Window *mpWindow;

	EventLog mEventLog;
	Boat mBoat;
	Ray mRay;
};