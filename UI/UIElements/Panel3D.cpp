#include "Panel3D.h"

Panel3D::Panel3D(int width, int height, int top, int left, HWND handle, LPCTSTR title)
	:Panel(width, height, top, left, handle), mDirect3D(width, height)
{
	// Creating a child window that will be the canvas to draw on for the panel.
	this->mPanelWindow = CreateWindowEx(
		0,
		title, 
		title, 
		WS_CHILD | WS_BORDER, 
		this->mLeft, 
		this->mTop, 
		this->mWidth, 
		this->mHeight, 
		handle, 
		0, 
		GetModuleHandle(0),
		0);	
	this->mDirect3D.Init(this->mPanelWindow);
	ShowWindow(this->mPanelWindow, SW_NORMAL);
}

Panel3D::~Panel3D()
{
}

D3D11 & Panel3D::rGetDirect3D()
{
	return this->mDirect3D;
}

const bool Panel3D::AddMeshObject(
	std::string name,
	std::vector<std::vector<unsigned int>> indices, 
	std::vector<std::vector<Vertex>> vertices)
{
	bool result = false;

	// Pushing a fresh mesh object to the vector to avoid
	// the destructor destroying the mesh object when out
	// of scope. Adding buffers to the last element of
	// the mesh object vector.
	this->mMeshObjects.push_back(MeshObject(name, indices, vertices));
	for (int i = 0; i < this->mMeshObjects.back().GetNumberOfBuffers(); i++)
	{

		this->mMeshObjects.back().AddIndexBuffer();
		this->mMeshObjects.back().AddVertexBuffer();
		this->CreateIndexBuffer(indices[i], i);
		this->CreateVertexBuffer(vertices[i], i);
	}

	return result;
}

bool Panel3D::CreateShadersAndSetup(
	LPCWSTR					 vertexShaderPath, 
	LPCWSTR					 geometryShaderPath, 
	LPCWSTR					 pixelShaderPath, 
	ID3D11VertexShader **	 pVertexshader, 
	ID3D11GeometryShader **  pGeometryShader, 
	ID3D11PixelShader **	 pPixelShader, 
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[], 
	UINT					 nrOfElements, 
	ID3D11InputLayout **	 pInputLayout)
{
	bool result = this->mDirect3D.CreateShaders(
		vertexShaderPath,
		geometryShaderPath,
		pixelShaderPath,
		pVertexshader,
		pGeometryShader,
		pPixelShader,
		inputElementDesc,
		nrOfElements,
		pInputLayout);

	// Setting shaders to the pipeline.
	this->mDirect3D.GetContext()->VSSetShader(*pVertexshader, nullptr, 0);
	this->mDirect3D.GetContext()->GSSetShader(*pGeometryShader, nullptr, 0);
	this->mDirect3D.GetContext()->PSSetShader(*pPixelShader, nullptr, 0);

	// Setting up input assembler.
	this->mDirect3D.GetContext()->IASetPrimitiveTopology
	(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->mDirect3D.GetContext()->IASetInputLayout(*pInputLayout);

	return result;
}

const void Panel3D::CreateVertexBuffer(
	std::vector<Vertex> vertices,
	int index)
{
	ID3D11Buffer *vertex_buffer = nullptr;

	// Assuming all meshes have the same format.
	D3D11_BUFFER_DESC vertex_buffer_desc{};
	vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_buffer_desc.ByteWidth = (UINT)(sizeof(Vertex) * vertices.size());
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertex_buffer_data{};
	vertex_buffer_data.pSysMem = vertices.data();

	if (FAILED(this->mDirect3D.GetDevice()->CreateBuffer(
		&vertex_buffer_desc,
		&vertex_buffer_data,
		&vertex_buffer)))
	{
		MessageBoxA(NULL, "Vertex buffer creation failed.", NULL, MB_OK);
		exit(-1);
	}
	this->mMeshObjects.back().SetVertexBuffer(&vertex_buffer);
}

const void Panel3D::CreateIndexBuffer(
	std::vector<unsigned int> indices, 
	int index)
{
	ID3D11Buffer *index_buffer = nullptr;
	
	// Assuming the index buffers follow the same format.
	D3D11_BUFFER_DESC index_buffer_desc{};
	index_buffer_desc.Usage		= D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth	= (UINT)(sizeof(unsigned int) * indices.size());
	index_buffer_desc.BindFlags	= D3D11_BIND_INDEX_BUFFER;
	
	D3D11_SUBRESOURCE_DATA index_buffer_data{};
	index_buffer_data.pSysMem = indices.data();

	if (FAILED(this->mDirect3D.GetDevice()->CreateBuffer(
		&index_buffer_desc, 
		&index_buffer_data, 
		&index_buffer)))
	{
		MessageBoxA(NULL, "Index buffer creation failed.", NULL, MB_OK);
		exit(-1);
	}
	this->mMeshObjects.back().SetIndexBuffer(&index_buffer);
}

const void Panel3D::Update()
{
	// TODO: Update the panel?
}

const void Panel3D::Draw()
{
	this->mDirect3D.Clear();

	// Stride (vertex size) and offset are
	// declared because they have to be referenced.
	UINT stride = (UINT)sizeof(Vertex);
	UINT offset = 0;

	// For readability.
	ID3D11Buffer* vertex_buffer = nullptr;
	ID3D11Buffer* index_buffer	= nullptr;
	UINT numIndices = 0;

	// Takes every set of buffers from every mesh object in the panel
	// and draws them one by one.
	for (int i = 0; i < (int)this->mMeshObjects.size(); i++)
	{
		for (int j = 0; j < this->mMeshObjects[i].GetNumberOfBuffers(); j++)
		{
			index_buffer	= *this->mMeshObjects[i].pGetIndexBuffer(j);
			vertex_buffer	= *this->mMeshObjects[i].pGetVertexBuffer(j);

			this->mDirect3D.GetContext()->IASetVertexBuffers(
				0,				// Start slot.
				1,				// Number of buffers.
				&vertex_buffer,	// Vertex buffer. 
				&stride,		// Vertex size.
				&offset);		// Offset.	

			this->mDirect3D.GetContext()->IASetIndexBuffer(
				index_buffer,			// Index buffer.
				DXGI_FORMAT_R32_UINT,	// Format.
				offset);				// Offset.

			numIndices = (UINT)this->mMeshObjects[i].GetIndices()[j].size();

			this->mDirect3D.GetContext()->DrawIndexed(
				numIndices,	// Number of indices.
				0,			// Start index location.
				offset);	// Base vertex location.
		}
	}
	this->mDirect3D.GetSwapChain()->Present(1, 0);
}

MeshObject* Panel3D::GetMeshObject(std::string name)
{
	
	for (int i = 0; i < (int)this->mMeshObjects.size(); i++)
	{
		if (name == this->mMeshObjects[i].GetName())
		{
			return &this->mMeshObjects[i];
		}
	}
	return nullptr;
}
