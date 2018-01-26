#include "Test_Panel.h"

/*
	FÖR DEN SOM REVIEWAR DETTA

	Filerna som behöver kollas (utöver denna) är:
		Panel.h
		Panel.cpp
		Panel2D.h	(inte klar för det är inte möjligt innan textlådor och knappar är klara)
		Panel2D.cpp (--------------------------------||------------------------------------)
		Panel3D.h
		Panel3D.cpp
		MeshObject.h
		MeshObject.cpp

	Lycka till att merga btw lmao.
*/

void Test_Panel3D()
{
	Window window(L"Test_Window", 1280, 720);
	window.Open();
	Panel3D panel1(
		640,					// Width.
		360,					// Height.
		0,						// Top (y coordinate of top edge).
		0,						// Left (x coordinate of left edge).
		window.GetWindow());	// Window handle to parent window of the panel.

	Panel3D panel2(640, 360, 0, 640, window.GetWindow());


	int click_x = 960;
	int click_y = 30;

	// Panel::Intersects returns a bool that states whether or not the click intersects the panel.
	bool click_intersects_panel1 = panel1.Intersects(click_x, click_y); // false due to the x coordinate being outside of panel1's range. 
	bool click_intersects_panel2 = panel2.Intersects(click_x, click_y); // true.

	std::cout << "Panel 1 intersection bool: " << std::boolalpha << click_intersects_panel1 << std::endl;
	std::cout << "Panel 2 intersection bool: " << std::boolalpha << click_intersects_panel2 << std::endl;


	// Panel::IntersectionFraction (Emil's function). Check function definition to understand.
	Fraction frac1 = panel1.IntersectionFraction(click_x, click_y); 
	Fraction frac2 = panel2.IntersectionFraction(click_x, click_y);

	std::cout << "Panel 1 x fraction: " << frac1.x << std::endl;
	std::cout << "Panel 1 y fraction: " << frac1.y << std::endl;

	std::cout << "Panel 2 x fraction: " << frac2.x << std::endl;
	std::cout << "Panel 2 y fraction: " << frac2.y << std::endl;

	
	// Testing shader creation and setting as well as setting up the IA and drawing.
	ID3D11InputLayout *pInputLayout = nullptr;
	ID3D11VertexShader *pVS			= nullptr;
	ID3D11GeometryShader *pGS		= nullptr;
	ID3D11PixelShader *pPS			= nullptr;

	D3D11_INPUT_ELEMENT_DESC input_desc = {
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA
	};
	
	// Triangle vertices for drawing and mesh object test.
	std::vector<Vertex> vertices;
	
	Vertex v1 = {
		-0.5f, -0.5f, 0.5f, // Position x, y, z
		0.0f, 0.0f, 0.0f,	// Normal x, y, z
		0.0f, 0.0f			// TexCoord u, v
	};
	Vertex v2 = {
		0.0f, 0.5f, 0.5f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f
	};
	Vertex v3 = {
		0.5f, -0.5f, 0.5f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f
	};

	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	// A vector of vectors because Rikard.
	std::vector<std::vector<unsigned int>> indices;

	std::vector<unsigned int> i1;
	i1.push_back(1);
	i1.push_back(2);
	i1.push_back(3);

	panel1.AddMeshObject(indices, vertices);
	panel2.AddMeshObject(indices, vertices);

	// Sick function bro.
	panel1.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl",
		&pVS,
		&pGS,
		&pPS,
		&input_desc,
		(UINT)1,
		&pInputLayout);

	panel2.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl",
		&pVS,
		&pGS,
		&pPS,
		&input_desc,
		(UINT)1,
		&pInputLayout);

	//panel1.Draw();
	if (pVS)
	{
		pVS->Release();
	}
	if (pGS)
	{
		pGS->Release();
	}
	if (pPS)
	{
		pPS->Release();
	}
	if (pInputLayout)
	{
		pInputLayout->Release();
	}

}

void Test_Panel2D()
{
	// TODO: Test 2D panel when finished.
}

