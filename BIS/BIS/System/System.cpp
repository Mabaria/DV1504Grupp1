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
	delete this->mpActiveLogPanel;	
	delete this->mpControlPanel;	
	delete this->mpSideViewPanel;	
	delete this->mpTopViewPanel;	
	delete this->mpTopViewCamera;
	delete this->mpSideViewCamera;	
	delete this->mpMenuPanel;	
	delete this->mpWindow;	

	
	for (int i = 0; i < (int)this->mFloors.size(); i++)
	{
		delete this->mFloors[i];	
	}
	for (int i = 0; i < (int)this->mBounds.size(); i++)
	{	
		delete this->mBounds[i];		
	}

	for (int i = 0; i < (int)this->mTexts.size(); i++)
	{	
		delete this->mTexts[i];		
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
		(float)this->mpTopViewPanel->GetWidth(), 
		(float)this->mpTopViewPanel->GetHeight(), 
		&this->mEventLog, 
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
	this->mpInfoPanel.Update();
}

void System::mDraw()
{
	this->mpActiveLogPanel->Draw();
	this->mpControlPanel->Draw();
	this->mpTopViewPanel->Draw();
	this->mpSideViewPanel->Draw();
	this->mpMenuPanel->Draw();
	this->mpInfoPanel.Draw();
}

void System::mHandleInput()
{
	if (Mouse::IsButtonPressed(Buttons::Left))
	{
		if (this->mpTopViewPanel->IsMouseInsidePanel() && 
			!this->mpMenuPanel->IsMouseInsidePanel() &&
			!this->mpInfoPanel.IsMouseInsidePanel())
		{
			Picking::GetWorldRay(
				this->mpTopViewCamera,
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
	
}

void System::mUpdateEvents(Room * room)
{
	std::vector<LogEvent*> events_in_room = room->GetActiveEvents();
	// If there already is an active event of that type in that room
	// the event is removed.
	if (!this->mpActiveLogPanel->AddNotification(room, events_in_room.back()))
	{
		Event::Type to_remove = this->mpMenuPanel->GetLastClicked();
		this->mpActiveLogPanel->RemoveNotification(room, to_remove);
		room->ClearEvent(to_remove);
		events_in_room = room->GetActiveEvents();
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
		event_data.slots[i] = (float)events_in_room[i]->GetType() + 1;
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

	this->mpSideViewCamera = new Camera(
		{ -0.0251480788f, 1.28821635f, 3.78684092f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.000001f, 0.0f, 0.0f, 0.0f },
		120.f, 32.f/9.f,
		0.1f, 25.0f, LOOK_AT, PERSPECTIVE);

	this->mpTopViewPanel->SetCamera(this->mpTopViewCamera);
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
	this->mpControlPanel->LoadImageToBitmap(
		"../../Models/Info.png", 
		"Info");

	this->mpControlPanel->AddButton(70, 70, 10, 10,
		this->mpControlPanel->GetBitmapByName("Reset"), "Reset");
	this->mpControlPanel->AddButton(70, 70, 90, 10,
		this->mpControlPanel->GetBitmapByName("Reset"), "Reset2");
	this->mpControlPanel->AddButton(70, 70, 90, 90,
		this->mpControlPanel->GetBitmapByName("Info"), "Info");

	this->mpControlPanel->GetButtonByName("Reset")->
		AddObserver(this->mpSideViewPanel);
	this->mpControlPanel->GetButtonByName("Reset2")->
		AddObserver(this->mpTopViewPanel);

	this->mpControlPanel->GetButtonByName("Info")->
		AddObserver(&this->mpInfoPanel);

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

	// Setting up the active log panel. (top, left, titleFontSize, objectFontSize)
	int list_top = 0;
	int list_left = 0;
	int title_font_size = 40;
	int object_font_size = 14;

	this->mpActiveLogPanel->SetNotificationList(
		list_top, 
		list_left, 
		title_font_size,
		object_font_size);


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
	MeshObject bound_brygg("Bryggdäckbounds", this->mBounds[0]);
	MeshObject bound_huvud("Huvuddäckbounds", this->mBounds[1]);
	MeshObject bound_tross("Trossdäckbounds", this->mBounds[2]);

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

	this->mpSideViewPanel->rGetMeshObject("Bryggdäck")->Translate(0.05f, 0.9f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäck")->Translate(0.05f, 0.0f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäck")->Translate(0.05f, -0.9f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Bryggdäckbounds")->Translate(0.05f, 0.9f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäckbounds")->Translate(0.05f, 0.0f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäckbounds")->Translate(0.05f, -0.9f, 0.0f);

	this->mpSideViewPanel->rGetMeshObject("Bryggdäck")->Scale(0.5f, 0.5f, 0.5f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäck")->Scale(0.5f, 0.5f, 0.5f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäck")->Scale(0.5f, 0.5f, 0.5f);
	this->mpSideViewPanel->rGetMeshObject("Bryggdäckbounds")->Scale(0.5f, 0.5f, 0.53f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäckbounds")->Scale(0.5f, 0.5f, 0.53f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäckbounds")->Scale(0.5f, 0.5f, 0.53f);


}

void System::mSetupBoat()
{
	this->mBoat.ReadFile("../../SaveFiles/data.boat");

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

	/**
	*	Send corresponding pointers
	*/
	this->mBoat.SetEventLog(&this->mEventLog);
}
