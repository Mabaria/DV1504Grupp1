#pragma once

#include "../../../UI/UIElements/Panel2D.h"
#include "../../../UI/UIElements/Panel3D.h"
#include "../../../Logic/Boat/Boat.h"
#include "../../../GraphicsEngine/Quad.h"

/* TODO:
*Picking
*Kontrollknappar
*
*/




class System
{
public:
	System();
	~System();
	void BuildGraphicalUserInterface(const std::wstring windowName, const int windowWidth, const int windowHeight);
	void Run();

private:
	void mUpdate();
	void mDraw();
	void mAddEvent(Room *room, LogEvent *logEvent);
	void mRemoveEvent(Room *room, LogEvent *logEvent);

	void mSetupPanels();
	void mSetupModels();
	void mSetupBoat();

	Panel2D *mpControlPanel;
	Panel2D *mpActiveLogPanel;
	Panel2D *mpPopUpPanel;
			
	Panel3D *mpSideViewPanel;
	Panel3D *mpTopViewPanel;

	std::vector<Mesh*> mFloors;
	std::vector<Mesh*> mBounds;
	std::vector<Quad*> mTexts;

	Camera *mpTopViewCamera;
	Camera *mpSideViewCamera;
	Window *mpWindow;

	Boat mBoat;
};