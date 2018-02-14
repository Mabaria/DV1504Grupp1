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

	this->mpSideViewPanel = new Panel3D(
		windowWidth / 3,
		windowHeight / 6,
		0,
		0,
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
		windowWidth / 6,		// Width
		windowHeight,			// Height
		0,						// Top
		5 * windowWidth / 6,	// Left
		this->mpWindow->GetWindow(),
		windowName.c_str());
	this->mpControlPanel = new Panel2D(
		windowWidth / 2,
		windowHeight / 6,
		0,
		windowWidth / 3,
		this->mpWindow->GetWindow(),
		windowName.c_str());

	this->mpCamera = new Camera(
		{ 2.0f, 5.0f, 3.5f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		XM_PI / 15.0f, 16.0f / 9.0f,
		0.1f, 25.0f, LOOK_AT, PERSPECTIVE);

	this->mpSideViewPanel->SetCamera(this->mpCamera);
	this->mpTopViewPanel->SetCamera (this->mpCamera);

	this->mpSideViewPanel->CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	this->mpTopViewPanel->CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	this->mFloors.push_back(new Mesh("../../Models/Floor01.obj"));
	this->mFloors.push_back(new Mesh("../../Models/Floor1.obj"));
	this->mFloors.push_back(new Mesh("../../Models/Floor2.obj"));
	this->mBounds.push_back(new Mesh("../../Models/Bound01.obj"));
	this->mBounds.push_back(new Mesh("../../Models/Bound1.obj"));
	this->mBounds.push_back(new Mesh("../../Models/Bound2.obj"));

	MeshObject floor01("floor01", this->mFloors[0]);
	MeshObject floor1 ("floor1" , this->mFloors[1]);
	MeshObject floor2 ("floor2" , this->mFloors[2]);
	MeshObject bound01("bound01", this->mBounds[0]);
	MeshObject bound1 ("bound1"	, this->mBounds[1]);
	MeshObject bound2 ("bound2"	, this->mBounds[2]);

	this->mpTopViewPanel->AddMeshObject(&floor01);
	this->mpTopViewPanel->AddMeshObject(&floor1);
	this->mpTopViewPanel->AddMeshObject(&floor2);
	this->mpTopViewPanel->AddMeshObject(&bound01, L"../../Models/BlendColor.dds", true);
	this->mpTopViewPanel->AddMeshObject(&bound1, L"../../Models/BlendColor.dds", true);
	this->mpTopViewPanel->AddMeshObject(&bound2, L"../../Models/BlendColor.dds", true);

	EventData data = {
	1.0,
	2.0,
	4.0,
	3.0
	};
	this->mpTopViewPanel->rGetMeshObject("bound2")->SetEvent(data, this->mpTopViewPanel->rGetDirect3D().GetContext(), 3);
	this->mpSideViewPanel->AddMeshObject(&floor01);
	this->mpSideViewPanel->AddMeshObject(&floor1);
	this->mpSideViewPanel->AddMeshObject(&floor2);
	this->mpSideViewPanel->AddMeshObject(&bound01);
	this->mpSideViewPanel->AddMeshObject(&bound1);
	this->mpSideViewPanel->AddMeshObject(&bound2);

	float scale = 0.1f;
	this->mpTopViewPanel->rGetMeshObject("floor2") ->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("floor1") ->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("floor01")->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("bound2") ->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("bound1") ->Scale(scale, scale, scale);
	this->mpTopViewPanel->rGetMeshObject("bound01")->Scale(scale, scale, scale);

	this->mpTopViewPanel->rGetMeshObject("floor2") ->Translate(0.0f, 0.0f,  0.5f);
	this->mpTopViewPanel->rGetMeshObject("floor1") ->Translate(0.0f, 0.0f,  0.0f);
	this->mpTopViewPanel->rGetMeshObject("floor01")->Translate(0.0f, 0.0f, -0.5f);
	this->mpTopViewPanel->rGetMeshObject("bound2") ->Translate(0.0f, 0.0f,  0.5f);
	this->mpTopViewPanel->rGetMeshObject("bound1") ->Translate(0.0f, 0.0f,  0.0f);
	this->mpTopViewPanel->rGetMeshObject("bound01")->Translate(0.0f, 0.0f, -0.5f);

	this->mpSideViewPanel->rGetMeshObject("floor2") ->Scale(0.15f, 0.4f, 0.1f);
	this->mpSideViewPanel->rGetMeshObject("floor1") ->Scale(0.15f, 0.4f, 0.1f);
	this->mpSideViewPanel->rGetMeshObject("floor01")->Scale(0.15f, 0.4f, 0.1f);
	this->mpSideViewPanel->rGetMeshObject("bound2") ->Scale(0.15f, 0.4f, 0.1f);
	this->mpSideViewPanel->rGetMeshObject("bound1") ->Scale(0.15f, 0.4f, 0.1f);
	this->mpSideViewPanel->rGetMeshObject("bound01")->Scale(0.15f, 0.4f, 0.1f);

	this->mpSideViewPanel->rGetMeshObject("floor2") ->Translate(0.05f, -0.2f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("floor1") ->Translate(0.05f,  0.0f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("floor01")->Translate(0.05f,  0.2f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("bound2") ->Translate(0.05f, -0.2f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("bound1") ->Translate(0.05f,  0.0f, 0.0f);
	this->mpSideViewPanel->rGetMeshObject("bound01")->Translate(0.05f,  0.2f, 0.0f);

	this->mpControlPanel->AddTextbox(200, 60, 0, 105, "Kontrollpanel", "title");
	this->mpControlPanel->SetTextBoxFontSize(20);
	this->mpControlPanel->AddButton(70, 70, 30, 20, "../../Models/Button01.png", "Injury");
	this->mpControlPanel->AddButton(70,	70, 30, 90, "../../Models/Button02.png", "Gas");
	this->mpControlPanel->AddButton(70, 70, 30,	160, "../../Models/Button03.png", "Water");
	this->mpControlPanel->AddButton(70, 70, 30, 230, "../../Models/Button04.png", "Fire");

	this->mpActiveLogPanel->SetNotificationList(0, 0);	
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
