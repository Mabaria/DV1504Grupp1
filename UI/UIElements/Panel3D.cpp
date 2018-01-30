#include "Panel3D.h"

Panel3D::Panel3D(int width, int height, int top, int left, HWND handle, LPCTSTR title)
	:Panel(width, height, top, left, handle), mDirect3D(/*handle,*/ width, height)
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
	//this->mDirect3D = D3D11(handle, this->mWidth, this->mHeight);
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
	std::vector<std::vector<unsigned int>> indices, 
	std::vector<std::vector<Vertex>> vertices)
{
	bool result = false;
	MeshObject new_mesh_object = MeshObject(indices, vertices);
	
	// Creating buffers for the mesh object.
	for (int i = 0; i < new_mesh_object.GetNumberOfBuffers(); i++)
	{
		new_mesh_object.AddIndexBuffer(*this->CreateIndexBuffer(indices[i]));
		new_mesh_object.AddVertexBuffer(this->CreateVertexBuffer(vertices[i]));
	}

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

ID3D11Buffer** Panel3D::CreateVertexBuffer(std::vector<Vertex> vertices)
{
	ID3D11Buffer *temp_vertex_buffer = nullptr;

	// Assuming all meshes have the same format.
	D3D11_BUFFER_DESC vertex_buffer_desc{};
	vertex_buffer_desc.Usage	 = D3D11_USAGE_DEFAULT;
	vertex_buffer_desc.ByteWidth = (UINT)(sizeof(Vertex) * vertices.size());
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertex_buffer_data{};
	vertex_buffer_data.pSysMem = vertices.data();
	
	if (FAILED(this->mDirect3D.GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &temp_vertex_buffer)))
	{		
		MessageBoxA(NULL, "Vertex buffer creation failed.", NULL, MB_OK);
		exit(-1);
	}

	return &temp_vertex_buffer;
}

ID3D11Buffer** Panel3D::CreateIndexBuffer(std::vector<unsigned int> indices)
{
	ID3D11Buffer *temp_index_buffer = nullptr;
	
	// Assuming the index buffers follow the same format.
	D3D11_BUFFER_DESC index_buffer_desc{};
	index_buffer_desc.Usage		= D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth	= (UINT)(sizeof(unsigned int) * indices.size());
	index_buffer_desc.BindFlags	= D3D11_BIND_INDEX_BUFFER;
	
	D3D11_SUBRESOURCE_DATA index_buffer_data{};
	index_buffer_data.pSysMem = indices.data();

	if (FAILED(this->mDirect3D.GetDevice()->CreateBuffer(&index_buffer_desc, &index_buffer_data, &temp_index_buffer)))
	{
		MessageBoxA(NULL, "Index buffer creation failed.", NULL, MB_OK);
		exit(-1);
	}

	return &temp_index_buffer;
}

const void Panel3D::Update()
{
	// TODO: Update the panel?
}

const void Panel3D::Draw()
{
	this->mDirect3D.Clear();
	
	// Stride (vertex size) is declared because it has to be referenced.
	UINT stride = (UINT)sizeof(Vertex);
	UINT offset = 0;

	

	ID3D11Buffer* test = nullptr;

	D3D11_BUFFER_DESC test_desc{};
	test_desc.ByteWidth = (UINT)(sizeof(Vertex) * this->mMeshObjects[0].GetVertices().size());
	test_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	test_desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA test_data{};
	test_data.pSysMem = this->mMeshObjects[0].GetVertices().data();

	this->mDirect3D.GetDevice()->CreateBuffer(&test_desc, &test_data, &test);

	if (test == nullptr)
	{
		MessageBoxA(NULL, "TEST IS NULL", NULL, MB_OK);
	}
	if (this->mDirect3D.GetContext() == nullptr)
	{
		MessageBoxA(NULL, "Context is nullptr", NULL, MB_OK);
	}

	// For readability.
	UINT numIndices = 0;
	this->mDirect3D.GetContext()->IASetVertexBuffers(
		0,											// Start slot.
		1,											// Number of buffers.
		&test,	// Vertex buffer. 
		&stride,									// Vertex size.
		&offset);									// Offset.

	//for (int i = 0; i < this->mMeshObjects.size(); i++)
	//{
	//	// For each mesh object their vertex buffer is set.

	//	for (int j = 0; j < this->mMeshObjects[i].GetNumberOfIndexBuffers(); j++)
	//	{
	//		// For each sub-mesh in the mesh object the index buffer is set and the
	//		// sub-mesh is drawn.
	//		this->mDirect3D.GetContext()->IASetIndexBuffer(
	//			*this->mMeshObjects[i].pGetIndexBuffer(j),	// Index buffer.
	//			DXGI_FORMAT_R32_UINT,						// Format.
	//			offset);									// Offset.

	//		numIndices = (UINT)this->mMeshObjects[i].GetIndices()[j].size();
	//		//this->mDirect3D.GetContext()->DrawIndexed(
	//		//	numIndices,	// Number of indices.
	//		//	0,			// Start index location.
	//		//	offset);	// Base vertex location.
	//	}
	//}
	this->mDirect3D.GetContext()->Draw(3, 0);
	this->mDirect3D.GetSwapChain()->Present(1, 0);
}
