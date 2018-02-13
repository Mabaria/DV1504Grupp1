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

}

void System::BuildGraphicalUserInterface(const std::wstring windowName, const int windowWidth, const int windowHeight)
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

	this->mFloors.push_back(new Mesh("../../Models/Floor01.obj"));
	this->mFloors.push_back(new Mesh("../../Models/Floor1.obj"));
	this->mFloors.push_back(new Mesh("../../Models/Floor2Materials.obj"));

	MeshObject floor01("floor01", this->mFloors[0]);
	MeshObject floor1("floor1", this->mFloors[1]);
	MeshObject floor2("floor2", this->mFloors[2]);

	this->mpTopViewPanel->AddMeshObject(&floor01);
	this->mpTopViewPanel->AddMeshObject(&floor1);
	this->mpTopViewPanel->AddMeshObject(&floor2);

	this->mpSideViewPanel->AddMeshObject(&floor01);
	this->mpSideViewPanel->AddMeshObject(&floor1);
	this->mpSideViewPanel->AddMeshObject(&floor2);

	this->mpControlPanel->AddButton(
		70, 
		70, 
		30, 
		20, 
		"../../../Models/Button01.png",
		"Injury");
	this->mpControlPanel->AddButton(
		70, 
		70, 
		30, 
		90, 
		"../../../Models/Button02.png", 
		"Gas");
	this->mpControlPanel->AddButton(
		70, 
		70, 
		30, 
		160, 
		"../../../Models/Button03.png", "Water");
	this->mpControlPanel->AddButton(
		70, 
		70, 
		30, 
		230, 
		"../../../Models/Button04.png", 
		"Fire");

	this->mpCamera = new Camera(
		{ 2.0f, 5.0f, 3.5f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		XM_PI / 15.0f, 16.0f / 9.0f,
		0.1f, 25.0f, LOOK_AT, PERSPECTIVE);
	this->mpSideViewPanel->SetCamera(this->mpCamera);
	this->mpTopViewPanel->SetCamera(this->mpCamera);
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
