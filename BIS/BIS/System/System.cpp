#include "System.h"

System::System()
{
	this->mpActiveLogPanel	= nullptr;
	this->mpControlPanel	= nullptr;
	this->mpTopViewCamera	= nullptr;
	this->mpSideViewCamera	= nullptr;
	this->mpMenuPanel		= nullptr;
	this->mpSideViewPanel	= nullptr;
	this->mpTopViewPanel	= nullptr;
	this->mpWindow			= nullptr;
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
	if (this->mpTopViewCamera)
	{
		delete this->mpTopViewCamera;
		this->mpTopViewCamera = nullptr;
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
		if (this->mFloors[i])
		{
			delete this->mFloors[i];
			this->mFloors[i] = nullptr;
		}
	}
	for (int i = 0; i < (int)this->mBounds.size(); i++)
	{
		if (this->mBounds[i])
		{
			delete this->mBounds[i];
			this->mBounds[i] = nullptr;
		}
	}

	for (int i = 0; i < (int)this->mTexts.size(); i++)
	{
		if (this->mTexts[i])
		{
			delete this->mTexts[i];
			this->mTexts[i] = nullptr;
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
		&this->mEventLog, 
		windowName.c_str(), 
		this->mpTopViewPanel->GetPanelWindowHandle());
	this->mpMenuPanel->AddObserver(this);

	this->mSetupPanels();
	this->mSetupModels();
	this->mSetupBoat();
}

void System::Run()
{
	this->mpWindow->Open();
	while (this->mpWindow->IsOpen())
	{
		this->mUpdate();
		this->mHandleInput();
		this->mDraw();
	}
}

void System::Update(Room * pickedRoom)
{
	this->mUpdateEvents(pickedRoom);
}

void System::mUpdate()
{
	this->mpWindow->Update();
	this->mpActiveLogPanel->Update();
	this->mpControlPanel->Update();
	this->mpTopViewPanel->Update();
	this->mpSideViewPanel->Update();
	this->mpMenuPanel->Update();
}

void System::mDraw()
{
	this->mpActiveLogPanel->Draw();
	this->mpControlPanel->Draw();
	this->mpTopViewPanel->Draw();
	this->mpSideViewPanel->Draw();
	this->mpMenuPanel->Draw();
}

void System::mHandleInput()
{
	if (Mouse::IsButtonPressed(Buttons::Left))
	{
		if (
			this->mpTopViewPanel->IsMouseInsidePanel() && 
			!this->mpMenuPanel->IsMouseInsidePanel())
		{
			Picking::GetWorldRay(
				this->mpTopViewPanel->GetActiveCamera(),
				Mouse::GetXPercentage(),
				Mouse::GetYPercentage(),
				this->mRay);

			Room *picked_room = this->mBoat.GetPickedRoom(this->mRay);
			if (picked_room)
			{
				this->mpMenuPanel->OpenAt(picked_room);
			}
		}
	}
	
	if (Keyboard::IsKeyPressed(Keys::One))
	{
		if (this->mpTopViewPanel->GetActiveCamera() != this->mpTopViewCamera)
		{
			this->mpTopViewPanel->SetCamera(this->mpTopViewCamera);
		}
		else if (this->mpTopViewPanel->GetActiveCamera() != this->mpTopViewCameraPan)
		{
			this->mpTopViewPanel->SetCamera(this->mpTopViewCameraPan);
		}
			
	}
}

void System::mAddEvent(Room * room)
{
	std::vector<LogEvent*> events_in_room = room->GetActiveEvents();
	this->mpActiveLogPanel->AddNotification(room, events_in_room.back());

	//! Temp solution.
	std::string test = room->GetDeckName();
	test.replace(test.begin() + 5, test.end(), "bounds");

	// Saving things for readability.
	MeshObject *top_picked_deck = this->mpTopViewPanel->rGetMeshObject(test);
	MeshObject *side_picked_deck = this->mpSideViewPanel->rGetMeshObject(test);
	int index_in_deck = room->GetIndexInDeck();

	// Filling event data for bounding box coloring.
	EventData event_data = { 0 };
	for (int i = 0; (i < (int)events_in_room.size()) && (i < 4); i++)
	{
		event_data.slots[i] = events_in_room[i]->GetType() + 1;
	}

	top_picked_deck->SetEvent(
		event_data,
		this->mpTopViewPanel->rGetDirect3D().GetContext(),
		index_in_deck);

	side_picked_deck->SetEvent(
		event_data,
		this->mpSideViewPanel->rGetDirect3D().GetContext(),
		index_in_deck);
	
}

void System::mRemoveEvent(Room * room)
{
	std::vector<LogEvent*> events_in_room = room->GetActiveEvents();
	
	/*this->mpActiveLogPanel->RemoveNotification(room, events_in_room.back());
	room->ClearEvent(this->mpMenuPanel->GetLastClicked());
*/
	std::string test = room->GetDeckName();
	test.replace(test.begin() + 5, test.end(), "bounds");

	MeshObject *top_picked_deck = this->mpTopViewPanel->rGetMeshObject(test);
	MeshObject *side_picked_deck = this->mpSideViewPanel->rGetMeshObject(test);
	int index_in_deck = room->GetIndexInDeck();

	EventData event_data = { 0 };
	for (int i = 0; (i < (int)events_in_room.size()) && (i < 4); i++)
	{
		event_data.slots[i] = events_in_room[i]->GetType() + 1;
	}

	top_picked_deck->SetEvent(
		event_data,
		this->mpTopViewPanel->rGetDirect3D().GetContext(),
		index_in_deck);

	side_picked_deck->SetEvent(
		event_data,
		this->mpSideViewPanel->rGetDirect3D().GetContext(),
		index_in_deck);
}

void System::mUpdateEvents(Room * room)
{
	std::vector<LogEvent*> events_in_room = room->GetActiveEvents();
	if (!this->mpActiveLogPanel->AddNotification(room, events_in_room.back()))
	{
		Event::Type to_remove = this->mpMenuPanel->GetLastClicked();
		this->mpActiveLogPanel->RemoveNotification(room, to_remove);
		room->ClearEvent(to_remove);
		events_in_room = room->GetActiveEvents();
	}

	// Temp solution. Sorry
	std::string test = room->GetDeckName();
	test.replace(test.begin() + 5, test.end(), "bounds");

	// Saving things for readability.
	MeshObject *top_picked_deck = this->mpTopViewPanel->rGetMeshObject(test);
	MeshObject *side_picked_deck = this->mpSideViewPanel->rGetMeshObject(test);
	int index_in_deck = room->GetIndexInDeck();

	// Filling event data for bounding box coloring.
	EventData event_data = { 0 };
	for (int i = 0; (i < (int)events_in_room.size()) && (i < 4); i++)
	{
		event_data.slots[i] = events_in_room[i]->GetType() + 1;
	}

	top_picked_deck->SetEvent(
		event_data,
		this->mpTopViewPanel->rGetDirect3D().GetContext(),
		index_in_deck);

	side_picked_deck->SetEvent(
		event_data,
		this->mpSideViewPanel->rGetDirect3D().GetContext(),
		index_in_deck);
}

void System::mSetupPanels()
{
	// Creating and setting the cameras.
	this->mpTopViewCamera = new Camera (
		{ -0.02f, 6.19f, 2.99f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.00000001f, 0.0f, 0.0f },
		XM_PI / 15.0f, 16.0f / 9.0f,
		0.1f, 10.0f, LOOK_AT, PERSPECTIVE);

	this->mpTopViewCameraPan = new Camera(
		{ 0.0f, 7.0f, 0.15f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, -10.0f, -0.25f, 0.0f },
		XM_PI / 15.0f, 16.0f / 9.0f,
		0.1f, 20.0f, LOOK_TO, PERSPECTIVE);

	this->mpSideViewCamera = new Camera(
		{ -0.0251480788f, 1.28821635f, 3.78684092f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.000001f, 0.0f, 0.0f, 0.0f },
		120.f, 32.f/9.f,
		0.1f, 25.0f, LOOK_AT, PERSPECTIVE);

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
		this->mpControlPanel->GetWidth(), 
		60,
		0, 
		0, 
		"Kontrollpanel", 
		"title");
	this->mpControlPanel->GetTextBoxByName("title")->SetFontSize(40);
	this->mpControlPanel->GetTextBoxByName("title")->SetFontWeight
	(DWRITE_FONT_WEIGHT_ULTRA_BLACK);
	this->mpControlPanel->GetTextBoxByName("title")->SetTextAlignment
	(DWRITE_TEXT_ALIGNMENT_CENTER);

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


	this->mpControlPanel->AddButton(70, 70, 10, 10,
		this->mpControlPanel->GetBitmapByName("Reset"), "Reset");
	this->mpControlPanel->AddButton(70, 70, 90, 10,
		this->mpControlPanel->GetBitmapByName("Reset"), "Reset2");


	this->mpControlPanel->GetButtonByName("Reset")->
		AddObserver(this->mpSideViewPanel);
	this->mpControlPanel->GetButtonByName("Reset2")->
		AddObserver(this->mpTopViewPanel);

	// Setting up the active log panel.
	this->mpActiveLogPanel->SetNotificationList(0, 0);


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

	this->mTexts.push_back(new Quad(true));


	// Creating temporary mesh objects to pass to the 3D panels.
	MeshObject floor_brygg("Bryggdäck", this->mFloors[0]);
	MeshObject floor_huvud("Huvuddäck", this->mFloors[1]);
	MeshObject floor_tross("Trossdäck", this->mFloors[2]);
	MeshObject bound_brygg("Bryggbounds", this->mBounds[0]);
	MeshObject bound_huvud("Huvudbounds", this->mBounds[1]);
	MeshObject bound_tross("Trossbounds", this->mBounds[2]);

	this->mpTopViewPanel->AddMeshObject(&floor_brygg);
	this->mpTopViewPanel->AddMeshObject(&floor_huvud);
	this->mpTopViewPanel->AddMeshObject(&floor_tross);
	this->mpTopViewPanel->AddMeshObject(&bound_brygg, L"../../Models/BlendColor.dds", true);
	this->mpTopViewPanel->AddMeshObject(&bound_huvud, L"../../Models/BlendColor.dds", true);
	this->mpTopViewPanel->AddMeshObject(&bound_tross, L"../../Models/BlendColor.dds", true);
	

	this->mpTopViewPanel->AddMeshObject(
		"Text3D_Floor01",
		this->mTexts[0]->GetIndices(),
		this->mTexts[0]->GetVertices(),
		L"../../Models/d01.dds"
	); 
	
	this->mpTopViewPanel->AddMeshObject(
		"Text3D_Floor1", 
		this->mTexts[0]->GetIndices(), 
		this->mTexts[0]->GetVertices(), 
		L"../../Models/d1.dds"
	);
	
	this->mpTopViewPanel->AddMeshObject(
		"Text3D_Floor2",
		this->mTexts[0]->GetIndices(),
		this->mTexts[0]->GetVertices(),
		L"../../Models/d2.dds"
	);


	this->mpSideViewPanel->AddMeshObject(&floor_brygg);
	this->mpSideViewPanel->AddMeshObject(&floor_huvud);
	this->mpSideViewPanel->AddMeshObject(&floor_tross);
	this->mpSideViewPanel->AddMeshObject(&bound_brygg, L"../../Models/BlendColor.dds", true);
	this->mpSideViewPanel->AddMeshObject(&bound_huvud, L"../../Models/BlendColor.dds", true);
	this->mpSideViewPanel->AddMeshObject(&bound_tross, L"../../Models/BlendColor.dds", true);


	// Scaling and translating the mesh objects in the panels.
	float scale = 0.1f;


	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor01")->
		Scale(scale * 2.0f, scale * 1.2f, scale * 3.5f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor01")->
		Rotate(XM_PI / 2.0f, XM_PI, 0.0f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor01")->
		Translate(0.85f, 0.0f, -0.5f);


	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor1")->
		Scale(scale * 1.0f, scale * 1.2f, scale * 3.5f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor1")->
		Rotate(XM_PI / 2.0, XM_PI, 0.0f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor1")->
		Translate(0.85f, 0.0f, 0.05f);


	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor2")->
		Scale(scale * 1.0f, scale * 1.2f, scale * 3.5f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor2")->
		Rotate(XM_PI / 2.0f, XM_PI, 0.0f);

	this->mpTopViewPanel->rGetMeshObject("Text3D_Floor2")->
		Translate(0.85f, 0.0f, 0.55f);

	

	this->mpTopViewPanel->rGetMeshObject("Bryggdäck")  ->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Huvuddäck")  ->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Trossdäck")  ->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Bryggbounds")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Huvudbounds")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Trossbounds")->Scale(scale, scale, scale);

	this->mpTopViewPanel->rGetMeshObject("Bryggdäck")->Translate(0.0f, 0.0f, -0.5f);
	this->mpTopViewPanel->rGetMeshObject("Huvuddäck")->Translate(0.0f, 0.0f, 0.0f);
	this->mpTopViewPanel->rGetMeshObject("Trossdäck")->Translate(0.0f, 0.0f, 0.5f);
	this->mpTopViewPanel->rGetMeshObject("Bryggbounds")->Translate(0.0f, 0.0f, -0.5f);
	this->mpTopViewPanel->rGetMeshObject("Huvudbounds")->Translate(0.0f, 0.0f, 0.0f);
	this->mpTopViewPanel->rGetMeshObject("Trossbounds")->Translate(0.0f, 0.0f, 0.5f);

	this->mpSideViewPanel->rGetMeshObject("Bryggdäck")->Translate(0.05f, 0.9f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäck")->Translate(0.05f, 0.0f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäck")->Translate(0.05f, -0.9f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Bryggbounds")->Translate(0.05f, 0.9f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Huvudbounds")->Translate(0.05f, 0.0f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Trossbounds")->Translate(0.05f, -0.9f, 0.0f);

	this->mpSideViewPanel->rGetMeshObject("Bryggdäck")->Scale(0.5f, 0.5f, 0.5f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäck")->Scale(0.5f, 0.5f, 0.5f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäck")->Scale(0.5f, 0.5f, 0.5f);
	this->mpSideViewPanel->rGetMeshObject("Bryggbounds")->Scale(0.5f, 0.5f, 0.53f);
	this->mpSideViewPanel->rGetMeshObject("Huvudbounds")->Scale(0.5f, 0.5f, 0.53f);
	this->mpSideViewPanel->rGetMeshObject("Trossbounds")->Scale(0.5f, 0.5f, 0.53f);


}

void System::mSetupBoat()
{
	this->mBoat.ReadFile("../../SaveFiles/data.boat");

	// Creating the mesh list that 
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

	/**
	*	Send corresponding pointers
	*/
	this->mBoat.SetEventLog(&this->mEventLog);
}
