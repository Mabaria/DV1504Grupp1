#include "Panel3D.h"

Panel3D::Panel3D(int width, int height, int top, int left, HWND handle)
	:Panel(width, height, top, left, handle), mDirect3D(handle, width, height)
{
	// Creating a child window that will be the canvas to draw on for the panel.
	this->mPanelWindow = CreateWindowEx(
		0,
		L"PanelWindowClass", 
		(LPCTSTR) NULL, 
		WS_CHILD | WS_BORDER, 
		this->mLeft, 
		this->mTop, 
		this->mWidth, 
		this->mHeight, 
		handle, 
		0, 
		GetModuleHandle(0),
		0);	
}

Panel3D::~Panel3D()
{
}

D3D11 & Panel3D::rGetDirect3D()
{
	return this->mDirect3D;
}

const bool Panel3D::AddMeshObject(
	std::vector<std::vector<unsigned int>> indices, 
	std::vector<Vertex> vertices)
{
	bool result = false;
	MeshObject new_mesh_object = MeshObject(indices, vertices);
	
	// Creating buffers for the mesh object.
	for (int i = 0; i < indices.size(); i++)
	{
		this->CreateIndexBuffer(indices[i], *new_mesh_object.pGetIndexBuffer(i));
	}
	this->CreateVertexBuffer(vertices, *new_mesh_object.pGetVertexBuffer());

	// Pushing the mesh object into the vector of mesh objects.
	this->mMeshObjects.push_back(new_mesh_object);

	return result;
}

bool Panel3D::CreateShadersAndSetup(
	LPCWSTR vertexShaderPath, 
	LPCWSTR geometryShaderPath, 
	LPCWSTR pixelShaderPath, 
	ID3D11VertexShader ** pVertexshader, 
	ID3D11GeometryShader ** pGeometryShader, 
	ID3D11PixelShader ** pPixelShader, 
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[], 
	UINT nrOfElements, 
	ID3D11InputLayout ** pInputLayout)
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
	this->mDirect3D.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->mDirect3D.GetContext()->IASetInputLayout(*pInputLayout);

	return result;
}

const bool Panel3D::CreateVertexBuffer(
	std::vector<Vertex> vertices, 
	ID3D11Buffer *vertexBuffer)
{
	bool result = true;

	// Assuming all meshes have the same format.
	D3D11_BUFFER_DESC vertex_buffer_desc{};
	vertex_buffer_desc.Usage	 = D3D11_USAGE_DEFAULT;
	vertex_buffer_desc.ByteWidth = (UINT)(sizeof(Vertex) * vertices.size());
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertex_buffer_data{};
	vertex_buffer_data.pSysMem = vertices.data();

	if (FAILED(this->mDirect3D.GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &vertexBuffer)))
	{
		result = false;
	}

	return result;
}

const bool Panel3D::CreateIndexBuffer(
	std::vector<unsigned int> indices, 
	ID3D11Buffer * indexBuffer)
{
	bool result = true;

	// Assuming the index buffers follow the same format.
	D3D11_BUFFER_DESC index_buffer_desc{};
	index_buffer_desc.Usage		= D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth	= (UINT)(sizeof(unsigned int) * indices.size());
	index_buffer_desc.BindFlags	= D3D11_BIND_INDEX_BUFFER;
	
	D3D11_SUBRESOURCE_DATA index_buffer_data{};
	index_buffer_data.pSysMem = indices.data();

	if (FAILED(this->mDirect3D.GetDevice()->CreateBuffer(&index_buffer_desc, &index_buffer_data, &indexBuffer)))
	{
		result = false;
	}

	return result;
}

const void Panel3D::Update()
{
	// TODO: Update the panel?
}

const void Panel3D::Draw()
{
	// Stride (vertex size) is declared because it has to be referenced.
	UINT stride = (UINT)sizeof(Vertex);

	// For readability.
	UINT numIndices = 0;

	for (int i = 0; i < this->mMeshObjects.size(); i++)
	{
		// For each mesh object their vertex buffer is set.
		this->mDirect3D.GetContext()->IASetVertexBuffers(
			0,											// Start slot.
			1,											// Number of buffers.
			this->mMeshObjects[i].pGetVertexBuffer(),	// Vertex buffer. 
			&stride,									// Vertex size.
			0);											// Offset.

		for (int j = 0; j < this->mMeshObjects[i].GetNumberOfIndexBuffers(); i++)
		{
			// For each sub-mesh in the mesh object the index buffer is set and the
			// sub-mesh is drawn.
			this->mDirect3D.GetContext()->IASetIndexBuffer(
				*this->mMeshObjects[i].pGetIndexBuffer(j),	// Index buffer.
				DXGI_FORMAT_R32_UINT,						// Format.
				0);											// Offset.

			numIndices = (UINT)this->mMeshObjects[i].GetIndices()[j].size();
			this->mDirect3D.GetContext()->DrawIndexed(
				numIndices,	// Number of indices.
				0,			// Start index location.
				0);			// Base vertex location.
		}
	}
}
