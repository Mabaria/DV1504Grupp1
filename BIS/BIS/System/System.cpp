#include "System.h"

System::System()
{
	this->mpActiveLogPanel	= nullptr;
	this->mpControlPanel	= nullptr;
	this->mpCamera			= nullptr;
	this->mpPopUpPanel		= nullptr;
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
	if (this->mpCamera)
	{
		delete this->mpCamera;
		this->mpCamera = nullptr;
	}
	if (this->mpPopUpPanel)
	{
		delete this->mpPopUpPanel;
		this->mpPopUpPanel = nullptr;
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
		windowName.c_str());
	this->mpTopViewPanel = new Panel3D(
		5 * windowWidth / 6,
		5 * windowHeight / 6,
		windowHeight / 6,
		0,
		this->mpWindow->GetWindow(),
		windowName.c_str());
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

	this->mSetupPanels();
	this->mSetupModels();
	this->mSetupBoat();

	/*EventData data = {
	1.0,
	2.0,
	4.0,
	3.0
	};
	this->mpTopViewPanel->rGetMeshObject("bound2")->SetEvent(data, this->mpTopViewPanel->rGetDirect3D().GetContext(), 3);*/

	
}

void System::Run()
{
	this->mpWindow->Open();
	while (this->mpWindow->IsOpen())
	{
		this->mUpdate();
		this->mDraw();
	}
}

void System::mUpdate()
{
	this->mpWindow->Update();
	this->mpActiveLogPanel->Update();
	this->mpControlPanel->Update();
	this->mpTopViewPanel->Update();
	this->mpSideViewPanel->Update();	
}

void System::mDraw()
{
	this->mpActiveLogPanel->Draw();
	this->mpControlPanel->Draw();
	this->mpTopViewPanel->Draw();
	this->mpSideViewPanel->Draw();
}

void System::mAddEvent(Room * room, LogEvent * logEvent)
{
	this->mpActiveLogPanel->AddNotification(room, logEvent);
	this->mpTopViewPanel->rGetMeshObject(room->GetDeckName());
}

void System::mSetupPanels()
{
	// Creating and setting the camera.
	this->mpCamera = new Camera(
		{ 2.0f, 5.0f, 3.5f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		XM_PI / 15.0f, 16.0f / 9.0f,
		0.1f, 25.0f, LOOK_AT, PERSPECTIVE);

	this->mpSideViewPanel->SetCamera(this->mpCamera);
	this->mpTopViewPanel->SetCamera(this->mpCamera);

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
	this->mpControlPanel->AddButton(70, 70, 30, 20, "../../Models/Button01.png", "Injury");
	this->mpControlPanel->AddButton(70, 70, 30, 90, "../../Models/Button02.png", "Gas");
	this->mpControlPanel->AddButton(70, 70, 30, 160, "../../Models/Button03.png", "Water");
	this->mpControlPanel->AddButton(70, 70, 30, 230, "../../Models/Button04.png", "Fire");
	this->mpControlPanel->AddButton(140, 140, 20, 600, "../../Models/Button05.png", "Reset");

	this->mpControlPanel->GetButtonByName("Reset")->AddObserver(this->mpTopViewPanel);

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
	this->mpTopViewPanel->AddMeshObject
	(&bound_brygg, L"../../Models/BlendColor.dds", true);
	this->mpTopViewPanel->AddMeshObject
	(&bound_huvud, L"../../Models/BlendColor.dds", true);
	this->mpTopViewPanel->AddMeshObject
	(&bound_tross, L"../../Models/BlendColor.dds", true);

	this->mpSideViewPanel->AddMeshObject(&floor_brygg);
	this->mpSideViewPanel->AddMeshObject(&floor_huvud);
	this->mpSideViewPanel->AddMeshObject(&floor_tross);
	this->mpSideViewPanel->AddMeshObject
	(&bound_brygg, L"../../Models/BlendColor.dds", true);
	this->mpSideViewPanel->AddMeshObject
	(&bound_huvud, L"../../Models/BlendColor.dds", true);
	this->mpSideViewPanel->AddMeshObject
	(&bound_tross, L"../../Models/BlendColor.dds", true);

	// Scaling and translating the mesh objects in the panels.
	float scale = 0.1f;
	this->mpTopViewPanel->rGetMeshObject("Bryggdäck")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Huvuddäck")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Trossdäck")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Bryggbounds")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Huvudbounds")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("Trossbounds")->Scale(scale, scale, scale);

	this->mpTopViewPanel->rGetMeshObject("Bryggdäck")->Translate(0.0f, 0.0f, -0.5f);
	this->mpTopViewPanel->rGetMeshObject("Huvuddäck")->Translate(0.0f, 0.0f, 0.0f);
	this->mpTopViewPanel->rGetMeshObject("Trossdäck")->Translate(0.0f, 0.0f, 0.5f);
	this->mpTopViewPanel->rGetMeshObject("Bryggbounds")->Translate(0.0f, 0.0f, -0.5f);
	this->mpTopViewPanel->rGetMeshObject("Huvudbounds")->Translate(0.0f, 0.0f, 0.0f);
	this->mpTopViewPanel->rGetMeshObject("Trossbounds")->Translate(0.0f, 0.0f, 0.5f);

	this->mpSideViewPanel->rGetMeshObject("Bryggdäck")->Scale(0.15f, 0.4f, 0.1f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäck")->Scale(0.15f, 0.4f, 0.1f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäck")->Scale(0.15f, 0.4f, 0.1f);
	this->mpSideViewPanel->rGetMeshObject("Bryggbounds")->Scale(0.15f, 0.4f, 0.1f);
	this->mpSideViewPanel->rGetMeshObject("Huvudbounds")->Scale(0.15f, 0.4f, 0.1f);
	this->mpSideViewPanel->rGetMeshObject("Trossbounds")->Scale(0.15f, 0.4f, 0.1f);

	this->mpSideViewPanel->rGetMeshObject("Bryggdäck")->Translate(0.05f, 0.2f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Huvuddäck")->Translate(0.05f, 0.0f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Trossdäck")->Translate(0.05f, -0.2f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Bryggbounds")->Translate(0.05f, 0.2f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Huvudbounds")->Translate(0.05f, 0.0f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("Trossbounds")->Translate(0.05f, -0.2f, 0.0f);
}

void System::mSetupBoat()
{

}
