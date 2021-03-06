#include "System.h"

System::System()
{
	this->mpActiveLogPanel	= nullptr;
	this->mpControlPanel	= nullptr;
	this->mpTopViewCameraRotate = nullptr;
	this->mpSideViewCamera	= nullptr;
	this->mpMenuPanel		= nullptr;
	this->mpSideViewPanel	= nullptr;
	this->mpTopViewPanel	= nullptr;
	this->mpWindow			= nullptr;
	this->mpLastClickedRoom  = nullptr;

}

System::~System()
{
	if (this->mpActiveLogPanel)
	{
		delete this->mpActiveLogPanel;
		this->mpActiveLogPanel = nullptr;
	}
	if (this->mpControlPanel)
	{
		delete this->mpControlPanel;
		this->mpControlPanel = nullptr;
	}
	if (this->mpSideViewPanel)
	{
		delete this->mpSideViewPanel;
		this->mpSideViewPanel = nullptr;
	}
	if (this->mpTopViewPanel)
	{
		delete this->mpTopViewPanel;
		this->mpTopViewPanel = nullptr;
	}
	if (this->mpTopViewCameraRotate)
	{
		delete this->mpTopViewCameraRotate;
		this->mpTopViewCameraRotate = nullptr;
	}
	if (this->mpTopViewCameraPan)
	{
		delete this->mpTopViewCameraPan;
		this->mpTopViewCameraPan = nullptr;
	}
	if (this->mpSideViewCamera)
	{
		delete this->mpSideViewCamera;
		this->mpSideViewCamera = nullptr;
	}
	if (this->mpMenuPanel)
	{
		delete this->mpMenuPanel;
		this->mpMenuPanel = nullptr;
	}
	if (this->mpWindow)
	{
		delete this->mpWindow;
		this->mpWindow = nullptr;
	}
	for (int i = 0; i < (int)this->mFloors.size(); i++)
	{
		delete this->mFloors[i];	
	}
	for (int i = 0; i < (int)this->mBounds.size(); i++)
	{
		if (this->mBounds[i])
		{
			delete this->mBounds[i];
			this->mBounds[i] = nullptr;
		}
	}
}

void System::BuildGraphicalUserInterface(
	const std::wstring windowName, 
	const int windowWidth, 
	const int windowHeight)
{
	this->mpWindow = new Window(
		windowName, 
		windowWidth, 
		windowHeight);

	// Creating the panels.
	this->mpSideViewPanel = new Panel3D(
		windowWidth / 3,	// Width
		windowHeight / 6,	// Height
		0,					// Top
		0,					// Left
		this->mpWindow->GetWindow(),
		windowName.c_str(),
		true);
	this->mpTopViewPanel = new Panel3D(
		5 * windowWidth / 6,
		5 * windowHeight / 6,
		windowHeight / 6,
		0,
		this->mpWindow->GetWindow(),
		windowName.c_str(),
		true);
	this->mpActiveLogPanel = new Panel2D(
		windowWidth / 6,
		windowHeight,
		0,
		5 * windowWidth / 6,
		this->mpWindow->GetWindow(),
		windowName.c_str());
	this->mpControlPanel = new Panel2D(
		windowWidth / 2,
		windowHeight / 6,
		0,
		windowWidth / 3,
		this->mpWindow->GetWindow(),
		windowName.c_str());

	this->mpMenuPanel = new EventMenu();
	this->mpMenuPanel->Init(
		this->mpTopViewPanel->GetWidth(), 
		this->mpTopViewPanel->GetHeight(), 
		windowName.c_str(), 
		this->mpTopViewPanel->GetPanelWindowHandle());
	this->mpMenuPanel->AddObserver(this);

	this->mpInfoPanel.Init(
		windowWidth / 3, 
		windowHeight - 100, 
		10,
		windowWidth / 3 + 10,
		this->mpWindow->GetWindow(),
		windowName.c_str());

	this->mpCrewPanel.Init(
		5 * windowWidth / 6,
		5 * windowHeight / 6,
		windowHeight / 6,
		0,
		this->mpWindow->GetWindow(),
		windowName.c_str());

	this->mSetupPanels();
	this->mSetupModels();
	this->mSetupBoat();

	this->mpWindow->Open();
}

bool System::Run()
{
	bool running = false;
	if (this->mpWindow->IsOpen())
	{
		this->mHandleInput();
		this->mUpdate();
		this->mDraw();
		running = true;
	}

	return running;
}

void System::Update(ObserverInfo * obsInf)
{
	if (obsInf->actionData == No_Action)
	{
		// If a room is clicked in top view panel.
		if (this->mpTopViewPanel->IsMouseInsidePanel())
		{
			this->mUpdateEvents(obsInf->pRoom, false);
		}
		// If a notification object is clicked in the active log panel.
		else if (this->mpActiveLogPanel->IsMouseInsidePanel())
		{
			this->mpMenuPanel->UpdateEventButtonImages();

			this->mUnselect();

			this->mpLastClickedRoom = obsInf->pRoom;

			if (this->mpTopViewPanel->GetActiveCamera()->GetLookMode() == LOOK_TO)
			{
				this->mpMenuPanel->OpenAt(this->mpLastClickedRoom);
			}

			this->mSelectAndFocus(this->mpLastClickedRoom);
		}
	}
}

int System::GetNrOfRooms() const
{
	return this->mBoat.GetRoomCount();
}

Room * System::GetRoomByIndex(int index)
{
	return this->mBoat.GetRoomPointerAt(index);
}

bool System::UpdateRoom(Room * room)
{
	this->mUpdateEvents(room, true);

	return true;
}

void System::mUpdate()
{
	if (!this->mpInfoPanel.IsMouseInsidePanel())
	{
		this->mpActiveLogPanel->Update();
		this->mpControlPanel->Update();
		this->mpTopViewPanel->Update();
		this->mpSideViewPanel->Update();
		this->mpCrewPanel.Update();
		this->mUpdateGhostIcons();
	}
	this->mpMenuPanel->Update();
	this->mpInfoPanel.Update();
	this->mpWindow->Update();
}

void System::mDraw()
{
	this->mpActiveLogPanel->Draw();
	this->mpControlPanel->Draw();
	this->mpTopViewPanel->Draw();
	this->mpSideViewPanel->Draw();
	this->mpMenuPanel->Draw();
	this->mpInfoPanel.Draw();
	this->mpCrewPanel.Draw();
}

void System::mHandleInput()
{
	static Room* last_picked_room = nullptr; 
	Room *picked_room = nullptr;

	// Only allows input if the cursor is within the top view panel,
	// but not within any other panel that overlaps the top view panel.
	// Also requires the look mode to be LOOK_TO because plotting should
	// not be done in LOOK_AT mode.
	if (this->mpTopViewPanel->IsMouseInsidePanel() &&
		!this->mpMenuPanel->IsMouseInsidePanel() &&
		!this->mpInfoPanel.IsMouseInsidePanel() &&
		!this->mpCrewPanel.IsMouseInsidePanel())
	{

		Picking::GetWorldRay(
			this->mpTopViewPanel->GetActiveCamera(),
			Mouse::GetXPercentage(),
			Mouse::GetYPercentage(),
			this->mRay);

		// Returns nullptr if picked position isn't a room
		picked_room = this->mBoat.GetPickedRoom(this->mRay);
		
		if (Mouse::IsButtonPressed(Buttons::Left) && this->mActionHandler.IsWaiting())
		{
			if (picked_room)
			{
				XMFLOAT3 picked_position = this->mBoat.GetPickedPosition(this->mRay);
				ActionHandler::Info result = this->mActionHandler.AddAction(
					picked_position.x,
					picked_position.z);


				if (result.ActionPtr != nullptr)
				{
					LogAction::Desc desc;

					desc.active = true;
					desc.start = true;
					desc.pActionIndex = result.ActionPtr;
					desc.pos_x = result.pos_x;
					desc.pos_z = result.pos_z;
					desc.type = (LogAction::Type)result.type;
					desc.data = result.data;

					picked_room->AddAction(desc);
				}
			}
			this->mActionHandler.SwitchWaitingState();
		}
		// Temp action removal.
		else if (Mouse::IsButtonPressed(Buttons::Left))
		{
			if (picked_room)
			{
				
				Actions *actions = this->mpTopViewPanel->pGetActions();
				int *act_ptr = actions->PickAction();

				if (act_ptr)
				{
					picked_room->ClearAction(act_ptr);
					actions->RemoveAction(&act_ptr);
				}
			}
		}
		// If actionHandler is ready to place an action, right click rotates the pending
		// action to be placed
		if (Mouse::IsButtonPressed(Buttons::Right) && this->mActionHandler.IsWaiting())
		{
			this->mActionHandler.RotatePendingAction();
		}

		if (picked_room)
		{
			if (Mouse::IsButtonPressed(Buttons::Left))
			{
				this->mUnselect();

				this->mpLastClickedRoom = picked_room;

				// Only open menu panel if pan mode.
				if (this->mpTopViewPanel->GetActiveCamera()->GetLookMode() == LOOK_TO)
				{
					this->mpMenuPanel->OpenAt(this->mpLastClickedRoom);
				}

				this->mSelectAndFocus(this->mpLastClickedRoom);
			}
		}
		// Closes the event menu if the user left clicks away from a room
		// or the event menu.
		else if (Mouse::IsButtonPressed(Buttons::Left))
		{
			if (this->mpMenuPanel->IsVisible())
			{
				this->mpMenuPanel->Close();
			}
			this->mpTopViewPanel->GetActiveCamera()->Reset();
		}
		

		//// ___ HOVER EFFECT ___

		if (picked_room)
		{
			if(last_picked_room != nullptr)
			{
				std::string last_picked_name = last_picked_room->GetDeckName() + "bounds";
				this->mUpdateHover(last_picked_name, last_picked_room->GetIndexInDeck(), false);
			}

			// Turn on hover effect for picked room
			std::string picked_name = picked_room->GetDeckName() + "bounds";
			this->mUpdateHover(picked_name, picked_room->GetIndexInDeck(), true);
		}

		else if(last_picked_room != nullptr)
		{
			std::string last_picked_name = last_picked_room->GetDeckName() + "bounds";
			this->mUpdateHover(last_picked_name, last_picked_room->GetIndexInDeck(), false);
		}

		last_picked_room = picked_room;

		//// ___ END ___ (HOVER EFFECT)
	}

	else if (last_picked_room != nullptr)
	{
		std::string last_picked_name = last_picked_room->GetDeckName() + "bounds";
		this->mUpdateHover(last_picked_name, last_picked_room->GetIndexInDeck(), false);
		last_picked_room = nullptr;
	}

}

void System::Update(Button * attribute)
{
	if (attribute)
	{
		if (attribute->GetName().compare("ChangeCamera") == 0)
		{
			if (this->mpTopViewPanel->GetActiveCamera() != this->mpTopViewCameraRotate)
			{
				this->mpTopViewPanel->SetCamera(this->mpTopViewCameraRotate);
				if (this->mpMenuPanel->IsVisible())
				{
					this->mpMenuPanel->Close();
				}
				if (this->mpLastClickedRoom)
				{
					this->mpTopViewPanel->rGetMeshObject(
						this->mpLastClickedRoom->GetDeckName() + "bounds")->SetSelected(
							false,
							this->mpTopViewPanel->rGetDirect3D().GetContext(),
							this->mpLastClickedRoom->GetIndexInDeck());
				}
			}
			else if (this->mpTopViewPanel->GetActiveCamera() != this->mpTopViewCameraPan)
			{
				this->mpTopViewPanel->SetCamera(this->mpTopViewCameraPan);
			}

			this->mpTopViewPanel->GetActiveCamera()->Reset();
		}

		else if (attribute->GetName().compare("Exit") == 0)
		{
			this->mpWindow->Close();
		}
		else if (attribute->GetName().compare("Simulator") == 0)
		{
			static bool active = false;

			active = !active;
			std::string extension = active ? "On" : "Off";

			this->mpControlPanel->GetButtonByName("Simulator")->SetBitmap(
				this->mpControlPanel->GetBitmapByName("Simulation" + extension)
			);
			this->NotifyObservers(&std::string("simulator_(de)activate"));
		}
	}
}

void System::mUpdateHover(std::string name, int index, bool activate)
{
	this->mpTopViewPanel->rGetMeshObject(name)->SetHover(
		activate,
		this->mpTopViewPanel->rGetDirect3D().GetContext(),
		index);
}

void System::mUpdateRoomInfo()
{
	std::string new_info_text;
	new_info_text = this->mpLastClickedRoom->GetName() + "\n" +
		this->mpLastClickedRoom->GetDeckName() + "\n" +
		"Sensorer: ";


	this->mpControlPanel->GetButtonByName("fireSensor")->SetOpacity(0.0f);
	this->mpControlPanel->GetButtonByName("waterSensor")->SetOpacity(0.0f);
	this->mpControlPanel->GetButtonByName("gasSensor")->SetOpacity(0.0f);

	// Get the sensors in the room
	std::vector<Event::Type> sensors;
	this->mpLastClickedRoom->GetInputTypes(sensors);

	for (std::vector<Event::Type>::iterator it = sensors.begin(); it != sensors.end();
		it++)
	{
		if (*it == 1)
		{
			this->mpControlPanel->GetButtonByName("fireSensor")->SetOpacity(1.0f);
		}
		// Event 1 is currently injury
		else if (*it == 4)
		{
			this->mpControlPanel->GetButtonByName("waterSensor")->SetOpacity(1.0f);
		
		}
		else if (*it == 8)
		{
			this->mpControlPanel->GetButtonByName("gasSensor")->SetOpacity(1.0f);
		}

	}
	this->mpControlPanel->GetTextBoxByName("roominfo")->SetText(
		new_info_text);
}

void System::mUnselect()
{
	if (this->mpLastClickedRoom)
	{
		std::string picked_name = this->mpLastClickedRoom->GetDeckName() + "bounds";
		this->mpTopViewPanel->rGetMeshObject(picked_name)->SetSelected(
			false,
			this->mpTopViewPanel->rGetDirect3D().GetContext(),
			this->mpLastClickedRoom->GetIndexInDeck()
		);
	}
}

void System::mSelectAndFocus(Room * picked_room)
{
	this->mpTopViewPanel->GetMovableComponent()->FocusCameraOnRoom(picked_room, true);

	// Turn on selected effect if clicked room was actually a room
	if (picked_room != nullptr)
	{
		std::string picked_name = picked_room->GetDeckName() + "bounds";
		this->mpTopViewPanel->rGetMeshObject(picked_name)->SetSelected(
			true,
			this->mpTopViewPanel->rGetDirect3D().GetContext(),
			picked_room->GetIndexInDeck()
		);
	}

	// Save last clicked room to be used for Room Info
	if (this->mpLastClickedRoom != picked_room)
	{
		// Turn off selected effect if last room existed
		if (this->mpLastClickedRoom != nullptr)
		{
			std::string picked_name = this->mpLastClickedRoom->GetDeckName() + "bounds";
			this->mpTopViewPanel->rGetMeshObject(picked_name)->SetSelected(
				false,
				this->mpTopViewPanel->rGetDirect3D().GetContext(),
				this->mpLastClickedRoom->GetIndexInDeck()
			);
		}
	}
	this->mUpdateRoomInfo();

}

void System::mUpdateEvent(Room * pRoom, LogEvent * pEvent)
{
	// If there already is an active event of that type in that room
	// the event is removed.
	this->mpActiveLogPanel->AddNotification(pRoom, pEvent);

	// Adding the system as an observer to the newly added notification object.
	this->mpActiveLogPanel->
		GetNotificationList()->
		GetNotificationObjectByIndex(
			this->mpActiveLogPanel->GetNotificationList()->
			GetNumberOfNotificationObjects() - 1)->
		AddObserver(this);

	// Adds bounds to the deck name to get the name of the 
	// mesh object holding the bounding boxes for the deck.
	std::string bounds_name = pRoom->GetDeckName() + "bounds";

	// Saving things for readability.
	MeshObject *top_picked_deck = this->mpTopViewPanel->rGetMeshObject(bounds_name);
	MeshObject *side_picked_deck = this->mpSideViewPanel->rGetMeshObject(bounds_name);
	int index_in_deck = pRoom->GetIndexInDeck();

	std::vector<LogEvent*> events_in_room;
	pRoom->GetActiveEvents(events_in_room);

	// Filling event data for bounding box coloring.
	EventData event_data = { 0 };
	for (int i = 0; (i < (int)events_in_room.size()) && (i < 4); i++)
	{
		event_data.slots[i] = (float)Event::GetID(events_in_room[i]->GetType()) + 1;
	}

	top_picked_deck->SetEvent(
		event_data,
		this->mpTopViewPanel->rGetDirect3D().GetContext(),
		index_in_deck);

	side_picked_deck->SetEvent(
		event_data,
		this->mpSideViewPanel->rGetDirect3D().GetContext(),
		index_in_deck);

	this->mpMenuPanel->UpdateEventButtonImages();
}

void System::mUpdateEvents(Room * room, bool automatic_input)
{
	std::vector<LogEvent*> events_in_room;
	room->GetActiveEvents(events_in_room);
	// If there already is an active event of that type in that room
	// the event is removed.
	if (!this->mpActiveLogPanel->AddNotification(room, events_in_room.back(), automatic_input))
	{
		// If event added through manual input (not through a sensor)
		if (!automatic_input)
		{
			Event::Type to_remove = this->mpMenuPanel->GetLastClicked();
			this->mpActiveLogPanel->RemoveNotification(room, to_remove);
			room->ClearEvent(to_remove);
			room->GetActiveEvents(events_in_room);
		}
	}
	// Adding the system as an observer to the newly added notification object.
	else
	{
		this->mpActiveLogPanel->
			GetNotificationList()->
			GetNotificationObjectByIndex(
				this->mpActiveLogPanel->GetNotificationList()->
				GetNumberOfNotificationObjects() - 1)->
			AddObserver(this);
	}

	// Adds bounds to the deck name to get the name of the 
	// mesh object holding the bounding boxes for the deck.
	std::string bounds_name = room->GetDeckName() + "bounds";

	// Saving things for readability.
	MeshObject *top_picked_deck = this->mpTopViewPanel->rGetMeshObject(bounds_name);
	MeshObject *side_picked_deck = this->mpSideViewPanel->rGetMeshObject(bounds_name);
	int index_in_deck = room->GetIndexInDeck();

	// Filling event data for bounding box coloring.
	EventData event_data = { 0 };
	for (int i = 0; (i < (int)events_in_room.size()) && (i < 4); i++)
	{
		event_data.slots[i] = (float)Event::GetID(events_in_room[i]->GetType()) + 1;
	}

	top_picked_deck->SetEvent(
		event_data,
		this->mpTopViewPanel->rGetDirect3D().GetContext(),
		index_in_deck);

	side_picked_deck->SetEvent(
		event_data,
		this->mpSideViewPanel->rGetDirect3D().GetContext(),
		index_in_deck);

	this->mpMenuPanel->UpdateEventButtonImages();
}

void System::mUpdateGhostIcons()
{
	static bool is_reset = false;
	static bool is_set = false;
	uint32_t action = 9001;
	if (this->mActionHandler.IsWaiting() && 
		!this->mpMenuPanel->IsMouseInsidePanel() &&
		this->mpTopViewPanel->IsMouseInsidePanel())
	{
		this->mpTopViewPanel->SetActionHover(true);
		if (!is_set)
		{
			action = *this->mActionHandler.GetLastAction();
			this->mpTopViewPanel->SetIcon(action);
			is_set = true;
			is_reset = false;
		}
		if (Mouse::IsButtonPressed(Buttons::Right))
		{
			this->mpTopViewPanel->RotateIcon();
		}
	}
	else if (!this->mActionHandler.IsWaiting())
	{
		if (!is_reset)
		{
			this->mpTopViewPanel->SetActionHover(false);
			this->mpTopViewPanel->ResetIcon();
			is_reset = true;
			is_set = false;
		}
	}
	else
	{
		if (action != *this->mActionHandler.GetLastAction())
		{
			is_set = false;
		}
		this->mpTopViewPanel->SetActionHover(false);
	}
}

void System::mSetupPanels()
{
	// Adding action support for topView
	this->mpTopViewPanel->InitActions();

	// Creating and setting the cameras.
	this->mpTopViewCameraRotate = new Camera (
		{ -0.02f, 6.19f, 2.99f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.00000001f, 0.0f, 0.0f },
		XM_PI / 15.0f, 16.0f / 9.0f,
		0.1f, 11.0f, LOOK_AT, PERSPECTIVE);

	this->mpTopViewCameraPan = new Camera(
		{ 0.0f, 7.0f, 0.15f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, -10.0f, -0.25f, 0.0f },
		XM_PI / 15.0f, 16.0f / 9.0f,
		0.1f, 11.0f, LOOK_TO, PERSPECTIVE);

	this->mpSideViewCamera = new Camera(
		{ -0.0251480788f, 1.28821635f, 3.78684092f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.000001f, 0.0f, 0.0f, 0.0f },
		120.f, 32.f/9.f,
		0.1f, 8.0f, LOOK_AT, PERSPECTIVE);

	this->mpTopViewPanel->SetCamera(this->mpTopViewCameraPan);
	this->mpSideViewPanel->SetCamera(this->mpSideViewCamera);

	// Creating and setting the shaders.
	this->mpSideViewPanel->CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	this->mpTopViewPanel->CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	// Setting up the control panel.

	this->mpControlPanel->AddTextbox(
		this->mpControlPanel->GetWidth() / 2, 
		60,
		0, 
		this->mpControlPanel->GetWidth() / 2 + 20,
		"Markerat rum:", 
		"title");
	this->mpControlPanel->GetTextBoxByName("title")->SetFontSize(35);
	this->mpControlPanel->GetTextBoxByName("title")->SetFontWeight
	(DWRITE_FONT_WEIGHT_NORMAL);

	this->mpControlPanel->AddTextbox(
		this->mpControlPanel->GetWidth() / 2,
		120,
		50,
		this->mpControlPanel->GetWidth() / 2 + 20,
		"Inget rum markerat",
		"roominfo");

	this->mpControlPanel->GetTextBoxByName("roominfo")->SetFontSize(30);


	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/Button01.png",
		"Injury");
	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/Button02.png",
		"Gas");
	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/Button03.png",
		"Water");
	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/Button04.png",
		"Fire");
	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/Button05.png",
		"Reset");
	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/Info.png",
		"Info");
	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/Clipboard.png",
		"Crew");
	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/ChangeCamera.png",
		"ChangeCamera"
	);
	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/Exit.png",
		"Exit"
	);

	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/SimulationOff.png",
		"SimulationOff"
	);

	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/SimulationOn.png",
		"SimulationOn"
	);

	this->mpControlPanel->AddButton(30, 30,
		this->mpControlPanel->GetHeight() / 2 + 50,
		this->mpControlPanel->GetWidth() / 2 + 150,
		this->mpControlPanel->GetBitmapByName("Fire"), "fireSensor");
	this->mpControlPanel->GetButtonByName("fireSensor")->SetOpacity(0.0f);

	this->mpControlPanel->AddButton(30, 30,
		this->mpControlPanel->GetHeight() / 2 + 50,
		this->mpControlPanel->GetWidth() / 2 + 190,
		this->mpControlPanel->GetBitmapByName("Water"), "waterSensor");
	this->mpControlPanel->GetButtonByName("waterSensor")->SetOpacity(0.0f);

	this->mpControlPanel->AddButton(30, 30,
		this->mpControlPanel->GetHeight() / 2 + 50,
		this->mpControlPanel->GetWidth() / 2 + 230,
		this->mpControlPanel->GetBitmapByName("Gas"), "gasSensor");
	this->mpControlPanel->GetButtonByName("gasSensor")->SetOpacity(0.0f);

	int butt_size = this->mpControlPanel->GetHeight() / 2;
	int space = 0;

	this->mpControlPanel->AddButton(butt_size, butt_size, space, space,
		this->mpControlPanel->GetBitmapByName("Reset"), "Reset");
	this->mpControlPanel->AddButton(butt_size, butt_size, butt_size + space * 2, space,
		this->mpControlPanel->GetBitmapByName("Reset"), "Reset2");
	this->mpControlPanel->AddButton(butt_size, butt_size, butt_size + space * 2, butt_size + space * 2,
		this->mpControlPanel->GetBitmapByName("Info"), "Info");
	this->mpControlPanel->AddButton(butt_size, butt_size, butt_size + space * 2, butt_size * 2 + space * 3,
		this->mpControlPanel->GetBitmapByName("Crew"), "Crew");
	this->mpControlPanel->AddButton(butt_size, butt_size, space, butt_size + space * 2,
		this->mpControlPanel->GetBitmapByName("ChangeCamera"), "ChangeCamera");
	this->mpControlPanel->AddButton(butt_size, butt_size, space, butt_size * 2 + space * 3,
		this->mpControlPanel->GetBitmapByName("SimulationOff"), "Simulator");
	this->mpControlPanel->AddButton(butt_size, butt_size, space, this->mpControlPanel->GetWidth() - (butt_size + space),
		this->mpControlPanel->GetBitmapByName("Exit"), "Exit");

	this->mpControlPanel->GetButtonByName("Reset")->
		AddObserver(this->mpSideViewPanel);
	this->mpControlPanel->GetButtonByName("Reset2")->
		AddObserver(this->mpTopViewPanel);

	this->mpControlPanel->GetButtonByName("Info")->
		AddObserver(&this->mpInfoPanel);
	this->mpControlPanel->GetButtonByName("Crew")->
		AddObserver(&this->mpCrewPanel);

	this->mpControlPanel->GetButtonByName("ChangeCamera")
		->AddObserver(this);
	this->mpControlPanel->GetButtonByName("Simulator")
		->AddObserver(this);
	this->mpControlPanel->GetButtonByName("Exit")
		->AddObserver(this);

	// Setting up the active log panel. (top, left, titleFontSize, objectFontSize)

	this->mpActiveLogPanel->LoadImageToBitmap(
		"../../Models/Button01.png",
		"Injury");
	this->mpActiveLogPanel->LoadImageToBitmap(
		"../../Models/Button02.png",
		"Gas");
	this->mpActiveLogPanel->LoadImageToBitmap(
		"../../Models/Button03.png",
		"Water");
	this->mpActiveLogPanel->LoadImageToBitmap(
		"../../Models/Button04.png",
		"Fire");
	this->mpActiveLogPanel->LoadImageToBitmap(
		"../../Models/Button05.png",
		"Reset");

	int list_top = 0;
	int list_left = 0;
	int title_font_size = 40;
	int object_font_size = 26;

	this->mpActiveLogPanel->SetNotificationList(
		list_top, 
		list_left, 
		title_font_size,
		object_font_size);

	this->mActionHandler.Init(this->mpTopViewPanel->pGetActions());
	this->mpMenuPanel->AddObserver(&this->mActionHandler);

	this->mActionHandler.AddObserver(this->mpMenuPanel);
}

void System::mSetupModels()
{
	// Creating the meshes and saving them.
	this->mFloors.push_back(new Mesh("../../Models/Floor01.obj"));
	this->mFloors.push_back(new Mesh("../../Models/Floor1.obj"));
	this->mFloors.push_back(new Mesh("../../Models/Floor2.obj"));

	this->mBounds.push_back(new Mesh("../../Models/Bound01.obj"));
	this->mBounds.push_back(new Mesh("../../Models/Bound1.obj"));
	this->mBounds.push_back(new Mesh("../../Models/Bound2.obj"));


	// Creating temporary mesh objects to pass to the 3D panels.
	MeshObject floor_brygg("Bryggdäck", this->mFloors[0]);
	MeshObject floor_huvud("Huvuddäck", this->mFloors[1]);
	MeshObject floor_tross("Trossdäck", this->mFloors[2]);
	MeshObject bound_brygg("Bryggdäckbounds", this->mBounds[0]);
	MeshObject bound_huvud("Huvuddäckbounds", this->mBounds[1]);
	MeshObject bound_tross("Trossdäckbounds", this->mBounds[2]);

	this->mpTopViewPanel->AddMeshObject(&floor_brygg, PANEL3D_SHADER_BOAT);
	this->mpTopViewPanel->AddMeshObject(&floor_huvud, PANEL3D_SHADER_BOAT);
	this->mpTopViewPanel->AddMeshObject(&floor_tross, PANEL3D_SHADER_BOAT);
	this->mpTopViewPanel->AddMeshObject(&bound_brygg, PANEL3D_SHADER_EVENT, L"../../Models/BlendColor.dds", true);
	this->mpTopViewPanel->AddMeshObject(&bound_huvud, PANEL3D_SHADER_EVENT, L"../../Models/BlendColor.dds", true);
	this->mpTopViewPanel->AddMeshObject(&bound_tross, PANEL3D_SHADER_EVENT, L"../../Models/BlendColor.dds", true);
	
	Quad quad(true);

	this->mpTopViewPanel->AddMeshObject(
		"Text3D_Floor3",
		quad.GetIndices(),
		quad.GetVertices(), PANEL3D_SHADER_TEXT,
		L"../../Models/d3.dds"
	); 
	
	this->mpTopViewPanel->AddMeshObject(
		"Text3D_Floor2", 
		quad.GetIndices(),
		quad.GetVertices(), PANEL3D_SHADER_TEXT,
		L"../../Models/d2.dds"
	);
	
	this->mpTopViewPanel->AddMeshObject(
		"Text3D_Floor1",
		quad.GetIndices(),
		quad.GetVertices(), PANEL3D_SHADER_TEXT,
		L"../../Models/d1.dds"
	);


	this->mpSideViewPanel->AddMeshObject(&floor_brygg, PANEL3D_SHADER_BOAT);
	this->mpSideViewPanel->AddMeshObject(&floor_huvud, PANEL3D_SHADER_BOAT);
	this->mpSideViewPanel->AddMeshObject(&floor_tross, PANEL3D_SHADER_BOAT);
	this->mpSideViewPanel->AddMeshObject(&bound_brygg, PANEL3D_SHADER_EVENT, L"../../Models/BlendColor.dds", true);
	this->mpSideViewPanel->AddMeshObject(&bound_huvud, PANEL3D_SHADER_EVENT, L"../../Models/BlendColor.dds", true);
	this->mpSideViewPanel->AddMeshObject(&bound_tross, PANEL3D_SHADER_EVENT, L"../../Models/BlendColor.dds", true);


	// Scaling and translating the mesh objects in the panels.
	float scale = 0.1f;


	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor3")->
		Scale(scale * 1.0f, scale * 1.2f, scale * 3.5f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor3")->
		Rotate(XM_PI / 2.0f, XM_PI, 0.0f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor3")->
		Translate(0.85f, 0.0f, -0.45f);


	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor2")->
		Scale(scale * 1.0f, scale * 1.2f, scale * 3.5f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor2")->
		Rotate(XM_PI / 2.0, XM_PI, 0.0f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor2")->
		Translate(0.85f, 0.0f, 0.05f);


	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor1")->
		Scale(scale * 1.0f, scale * 1.2f, scale * 3.5f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor1")->
		Rotate(XM_PI / 2.0f, XM_PI, 0.0f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor1")->
		Translate(0.85f, 0.0f, 0.55f);

	

	this->mpTopViewPanel->rGetMeshObject("Bryggdäck")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Huvuddäck")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Trossdäck")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Bryggdäckbounds")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Huvuddäckbounds")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Trossdäckbounds")->Scale(scale, scale, scale);

	this->mpTopViewPanel->rGetMeshObject("Bryggdäck")->Translate(0.0f, 0.0f, -0.5f);
	this->mpTopViewPanel->rGetMeshObject("Huvuddäck")->Translate(0.0f, 0.0f, 0.0f);
	this->mpTopViewPanel->rGetMeshObject("Trossdäck")->Translate(0.0f, 0.0f, 0.5f);
	this->mpTopViewPanel->rGetMeshObject("Bryggdäckbounds")->Translate(0.0f, 0.0f, -0.5f);
	this->mpTopViewPanel->rGetMeshObject("Huvuddäckbounds")->Translate(0.0f, 0.0f, 0.0f);
	this->mpTopViewPanel->rGetMeshObject("Trossdäckbounds")->Translate(0.0f, 0.0f, 0.5f);

	this->mpSideViewPanel->rGetMeshObject("Bryggdäck")->Translate(0.05f, 1.3f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäck")->Translate(0.05f, 0.0f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäck")->Translate(0.05f, -1.3f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Bryggdäckbounds")->Translate(0.05f, 1.3f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäckbounds")->Translate(0.05f, 0.0f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäckbounds")->Translate(0.05f, -1.3f, 0.0f);

	this->mpSideViewPanel->rGetMeshObject("Bryggdäck")->Scale(0.5f, 0.5f, 0.5f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäck")->Scale(0.5f, 0.5f, 0.5f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäck")->Scale(0.5f, 0.5f, 0.5f);
	this->mpSideViewPanel->rGetMeshObject("Bryggdäckbounds")->Scale(0.5f, 0.5f, 0.53f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäckbounds")->Scale(0.5f, 0.5f, 0.53f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäckbounds")->Scale(0.5f, 0.5f, 0.53f);
}

void System::mSetupBoat()
{
	this->mBoat.LoadFromFile_Boat("../../Savefiles/Boats/Vulcanus.boat");

	this->mBoat.SetLogPath("../../Savefiles/Logs/Log.log");
	this->mBoat.SetLogMetaPath("../../Savefiles/Metafiles/Log.meta");
	this->mBoat.SetRoomMetaDir("../../Savefiles/Metafiles/RoomLogMetas/");

	// Creating the mesh and matrix list that boat 
	// needs to load bounding boxes to the rooms.
	Mesh mesh_list[] =
	{
		*this->mBounds[0], // Bryggdäck
		*this->mBounds[1], // Huvuddäck
		*this->mBounds[2], // Trossdäck
		*this->mFloors[0],
		*this->mFloors[1],
		*this->mFloors[2]
	};

	XMMATRIX *floor_matrix_list[] =
	{
		this->mpTopViewPanel->rGetMeshObject("Bryggdäck")->rGetModelMatrix(),
		this->mpTopViewPanel->rGetMeshObject("Huvuddäck")->rGetModelMatrix(),
		this->mpTopViewPanel->rGetMeshObject("Trossdäck")->rGetModelMatrix()
	};

	this->mBoat.LoadBoundingBoxes(mesh_list, floor_matrix_list, 3);



	std::vector<Log::ActionInfo> activeActions;
	std::vector<Actions::Info> actionInfos;
	std::vector<int*> actionPointers;

	// Load log and meta files
	this->mBoat.LoadFromFile_Log();

	// Get a vector of all active actions
	this->mBoat.GetAllActiveActions(activeActions);

	// Fill vector with information about all active actions
	for (int i = 0; i < (int)activeActions.size(); i++)
	{
		Actions::Info info;
		info.x = activeActions[i].pAction->GetPos_X();
		info.z = activeActions[i].pAction->GetPos_Z();
		info.data = activeActions[i].pAction->GetData();

		actionInfos.push_back(info);
	}

	// Create graphical actions of all active actions
	this->mActionHandler.InitFromFile(actionInfos, actionPointers);

	// Update all active actions with a pointer to their graphical action
	for (int i = 0; i < (int)activeActions.size(); i++)
		activeActions[i].pIndexPtr = actionPointers[i];

	this->mBoat.UpdateActionPointers(activeActions);


	/**
	*	Update notification list with all active events
	*/

	// Recieve all active events

	std::list<EventInfo> infolist;

	Room *pRoom;
	int size = this->mBoat.GetRoomCount();
	for (int i = 0; i < size; i++)
	{
		pRoom = this->mBoat.GetRoomPointerAt(i);

		if (pRoom->GetActiveEventCount() > 0)
		{
			std::vector<LogEvent*> events;
			pRoom->GetActiveEvents(events);

			EventInfo eventinfo;
			eventinfo.pRoom = pRoom;

			for (int j = 0; j < (int)events.size(); j++)
			{
				eventinfo.pEvent = events[j];

				infolist.push_back(eventinfo);
			}
		}
	}

	// Sort all events by time

	infolist.sort();

	// Fill active list

	EventInfo eventinfo;
	while ((int)infolist.size() > 0)
	{
		eventinfo = infolist.front();

		this->mpMenuPanel->SetActiveRoom(eventinfo.pRoom);
		this->mUpdateEvent(eventinfo.pRoom, eventinfo.pEvent);

		infolist.pop_front();
	}
}
