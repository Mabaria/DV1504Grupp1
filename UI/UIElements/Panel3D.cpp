#include "Panel3D.h"

Panel3D::Panel3D(int width, int height, int top, int left, HWND handle, LPCTSTR title)
	:Panel(width, height, top, left, handle), mDirect3D(width, height)
{
	// Creating a child window that will be 
	// the canvas to draw on for the panel.
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

	this->mpVertexShader	= nullptr;
	this->mpGeometryShader	= nullptr;
	this->mpPixelShader		= nullptr;
	this->mpInputLayout		= nullptr;	

	// Input element description did not want to be initialized any other way.
	this->mInputDesc[0] = 
	{ 
		"POSITION", 
		0, 
		DXGI_FORMAT_R32G32B32_FLOAT, 
		0, 
		0, 
		D3D11_INPUT_PER_VERTEX_DATA, 
		0
	};
	this->mInputDesc[1] = 
	{ 
		"NORMAL", 
		0, 
		DXGI_FORMAT_R32G32B32_FLOAT, 
		0, 
		12, 
		D3D11_INPUT_PER_VERTEX_DATA, 
		0 
	};
	this->mInputDesc[2] = 
	{ 
		"TEXCOORD", 
		0, 
		DXGI_FORMAT_R32G32_FLOAT, 
		0, 
		24, 
		D3D11_INPUT_PER_VERTEX_DATA, 
		0 
	};

	this->mpCamera		= nullptr;
	this->mpViewBuffer	= nullptr;
	this->mpProjBuffer	= nullptr;
}

Panel3D::~Panel3D()
{
	if (this->mpVertexShader)
	{
		this->mpVertexShader->Release();
		this->mpVertexShader = nullptr;
	}
	if (this->mpGeometryShader)
	{		  
		this->mpGeometryShader->Release();
		this->mpGeometryShader = nullptr;
	}
	if (this->mpPixelShader)
	{
		this->mpPixelShader->Release();
		this->mpPixelShader = nullptr;
	}
	if (this->mpInputLayout)
	{			
		this->mpInputLayout->Release();
		this->mpInputLayout = nullptr;
	}
	if (this->mpViewBuffer)
	{			
		this->mpViewBuffer->Release();
		this->mpViewBuffer = nullptr;
	}
	if (this->mpProjBuffer)
	{			
		this->mpProjBuffer->Release();
		this->mpProjBuffer = nullptr;
	}
	for (int i = 0; i < (int)this->mpMeshObjects.size(); i++)
	{
		if (this->mpMeshObjects[i] != nullptr)
		{
			delete this->mpMeshObjects[i];
			this->mpMeshObjects[i] = nullptr;
		}
	}
}

D3D11 & Panel3D::rGetDirect3D()
{
	return this->mDirect3D;
}

//const void Panel3D::AddMeshObject(
//	std::string name,
//	std::vector<std::vector<unsigned int>> indices, 
//	std::vector<std::vector<Vertex>> vertices,
//	std::wstring texturePath)
//{
//	MeshObject *mesh_object = new MeshObject(name, indices, vertices);
//	this->mpMeshObjects.push_back(mesh_object);
//
//	for (int i = 0; i < this->mpMeshObjects.back()->GetNumberOfBuffers(); i++)
//	{
//		this->CreateIndexBuffer(indices[i]);
//		this->CreateVertexBuffer(vertices[i]);
//	}
//	this->CreateConstantBuffer(
//		this->mpMeshObjects.back()->rGetModelMatrix(), 
//		this->mpMeshObjects.back()->rGetConstantBuffer());
//	if (texturePath != L"")
//	{
//		this->CreateTexture(texturePath);
//	}
//}

const void Panel3D::AddMeshObject(MeshObject * meshObject)
{
	MeshObject *mesh_object = new MeshObject(*meshObject);
	this->mpMeshObjects.push_back(mesh_object);

	for (int i = 0; i < this->mpMeshObjects.back()->GetNumberOfBuffers(); i++)
	{
		this->CreateIndexBuffer(meshObject->GetIndices()[i]);
		this->CreateVertexBuffer(meshObject->GetVertices()[i]);
	}
	this->CreateConstantBuffer(
		this->mpMeshObjects.back()->rGetModelMatrix(),
		this->mpMeshObjects.back()->rGetConstantBuffer());
}

bool Panel3D::CreateShadersAndSetup(
	LPCWSTR	vertexShaderPath, 
	LPCWSTR	geometryShaderPath, 
	LPCWSTR	pixelShaderPath)
{
	bool result = this->mDirect3D.CreateShaders(
		vertexShaderPath,
		geometryShaderPath,
		pixelShaderPath,
		&this->mpVertexShader,
		&this->mpGeometryShader,
		&this->mpPixelShader,
		this->mInputDesc,
		sizeof(mInputDesc)/sizeof(D3D11_INPUT_ELEMENT_DESC),
		&this->mpInputLayout);

	// Setting shaders to the pipeline.
	this->mDirect3D.GetContext()->VSSetShader(
		this->mpVertexShader, 
		nullptr, 
		0);
	this->mDirect3D.GetContext()->GSSetShader(
		this->mpGeometryShader, 
		nullptr, 
		0);
	this->mDirect3D.GetContext()->PSSetShader(
		this->mpPixelShader, 
		nullptr, 
		0);

	// Setting up input assembler.
	this->mDirect3D.GetContext()->IASetPrimitiveTopology
	(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->mDirect3D.GetContext()->IASetInputLayout(this->mpInputLayout);

	return result;
}

const void Panel3D::CreateVertexBuffer(std::vector<Vertex> vertices)
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
	this->mpMeshObjects.back()->AddVertexBuffer(&vertex_buffer);
}

const void Panel3D::CreateIndexBuffer(std::vector<unsigned int> indices)
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
	this->mpMeshObjects.back()->AddIndexBuffer(&index_buffer);
}

const void Panel3D::CreateConstantBuffer(
	XMMATRIX *matrix, 
	ID3D11Buffer **constantBuffer)
{
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage			= D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	buffer_desc.ByteWidth		= sizeof(XMMATRIX);

	D3D11_SUBRESOURCE_DATA buffer_data{};
	buffer_data.pSysMem = matrix;

	// Creates the buffer with the things.
	if (FAILED(this->mDirect3D.GetDevice()->CreateBuffer(
		&buffer_desc, 
		&buffer_data, 
		constantBuffer)))
	{
		MessageBoxA(NULL, "Error creating constant buffer.", NULL, MB_OK);
		exit(-1);
	}
}

const void Panel3D::CreateTexture(std::wstring texturePath)
{
	if (FAILED(CreateDDSTextureFromFile(
		this->mDirect3D.GetDevice(), 
		texturePath.c_str(), 
		nullptr, 
		this->mpMeshObjects.back()->rGetTextureView())))
	{
		MessageBoxA(NULL, "Error loading texture.", NULL, MB_OK);
		exit(-1);
	}
}

const void Panel3D::UpdateConstantBuffer(std::string name)
{
	// Getting the mesh object of the given name.
	MeshObject *mesh_object = this->rGetMeshObject(name);
	
	if (mesh_object)
	{
		// Getting the constant buffer and model matrix from that mesh object.
		ID3D11Buffer *constant_buffer = *mesh_object->rGetConstantBuffer();
		const XMMATRIX *model_matrix = mesh_object->rGetModelMatrix();

		// Mapping the buffer data to the CPU in order to change it,
		// then unmapping it again, which updates the buffer contents.
		D3D11_MAPPED_SUBRESOURCE data_ptr;
		this->mDirect3D.GetContext()->Map(
			constant_buffer,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&data_ptr);

		memcpy(data_ptr.pData, model_matrix, sizeof(XMMATRIX));
		this->mDirect3D.GetContext()->Unmap(constant_buffer, 0);
	}
}

const void Panel3D::UpdateConstantBuffer(
	XMMATRIX * matrix, 
	ID3D11Buffer ** buffer)
{
	if (matrix && *buffer)
	{
		// Mapping the buffer data to the CPU in order to change it,
		// then unmapping it again, which updates the buffer contents.
		D3D11_MAPPED_SUBRESOURCE data_ptr;
		this->mDirect3D.GetContext()->Map(
			*buffer,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&data_ptr);

		memcpy(data_ptr.pData, matrix, sizeof(XMMATRIX));
		this->mDirect3D.GetContext()->Unmap(*buffer, 0);
	}
}

const void Panel3D::UpdateConstantBuffer(int index)
{
	if (index > -1 && index < (int)this->mpMeshObjects.size())
	{
		// Getting the mesh object of the given name.
		MeshObject *mesh_object = this->mpMeshObjects[index];

		// Getting the constant buffer and model matrix from that mesh object.
		ID3D11Buffer *constant_buffer = *mesh_object->rGetConstantBuffer();
		const XMMATRIX *model_matrix = mesh_object->rGetModelMatrix();

		// Mapping the buffer data to the CPU in order to change it,
		// then unmapping it again, which updates the buffer contents.
		D3D11_MAPPED_SUBRESOURCE data_ptr{};
		this->mDirect3D.GetContext()->Map(
			constant_buffer,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&data_ptr);

		memcpy(data_ptr.pData, model_matrix, sizeof(XMMATRIX));
		this->mDirect3D.GetContext()->Unmap(constant_buffer, 0);
	}
}

const void Panel3D::SetCamera(Camera * camera)
{
	this->mpCamera = camera;

	this->CreateConstantBuffer(
		&this->mpCamera->GetViewMatrix(), 
		&this->mpViewBuffer);
	this->CreateConstantBuffer(
		&this->mpCamera->GetProjectionMatrix(), 
		&this->mpProjBuffer);
}

const void Panel3D::Update()
{
	// Updating the constant buffers of the mesh objects.
	for (int i = 0; i < (int)this->mpMeshObjects.size(); i++)
	{
		UpdateConstantBuffer(i);
	}
	if (this->mpCamera)
	{
		// Updating the camera buffers.
		UpdateConstantBuffer(
			&this->mpCamera->GetViewMatrix(),
			&this->mpViewBuffer);
		UpdateConstantBuffer(
			&this->mpCamera->GetProjectionMatrix(),
			&this->mpProjBuffer);
	}
}

const void Panel3D::Draw()
{
	this->mDirect3D.Clear();

	// Stride (vertex size) and offset are
	// declared because they have to be referenced.
	UINT stride = (UINT)sizeof(Vertex);
	UINT offset = 0;

	// For readability.
	ID3D11Buffer* vertex_buffer		= nullptr;
	ID3D11Buffer* index_buffer		= nullptr;
	ID3D11Buffer* constant_buffer	= nullptr;
	UINT numIndices = 0;

	this->mDirect3D.GetContext()->VSSetConstantBuffers(
		1,
		1,
		&this->mpViewBuffer
	);

	this->mDirect3D.GetContext()->VSSetConstantBuffers(
		2,
		1,
		&this->mpProjBuffer
	);

	// Takes every set of buffers from every mesh object in the panel
	// and draws them one by one.
	for (int i = 0; i < (int)this->mpMeshObjects.size(); i++)
	{
		constant_buffer = *this->mpMeshObjects[i]->rGetConstantBuffer();

		// Setting the constant buffer to the vertex shader.
		this->mDirect3D.GetContext()->VSSetConstantBuffers(
			0,					// Start slot.
			1,					// Number of buffers
			&constant_buffer);	// Constant buffer.

		for (int j = 0; j < this->mpMeshObjects[i]->GetNumberOfBuffers(); j++)
		{
			index_buffer	= *this->mpMeshObjects[i]->pGetIndexBuffer(j);
			vertex_buffer	= *this->mpMeshObjects[i]->pGetVertexBuffer(j);

			// Setting the index buffer to the input assembler.
			this->mDirect3D.GetContext()->IASetVertexBuffers(
				0,				// Start slot.
				1,				// Number of buffers.
				&vertex_buffer,	// Vertex buffer. 
				&stride,		// Vertex size.
				&offset);		// Offset.	

			// Setting the index buffer to the input assembler.
			this->mDirect3D.GetContext()->IASetIndexBuffer(
				index_buffer,			// Index buffer.
				DXGI_FORMAT_R32_UINT,	// Format.
				offset);				// Offset.

			numIndices = (UINT)this->mpMeshObjects[i]->GetIndices()[j].size();
			this->mDirect3D.GetContext()->DrawIndexed(
				numIndices,	// Number of indices.
				0,			// Start index location.
				offset);	// Base vertex location.
		}
	}
	this->mDirect3D.GetSwapChain()->Present(1, 0);
}

MeshObject* Panel3D::rGetMeshObject(std::string name)
{
	
	for (int i = 0; i < (int)this->mpMeshObjects.size(); i++)
	{
		if (name == this->mpMeshObjects[i]->GetName())
		{
			return this->mpMeshObjects[i];
		}
	}
	return nullptr;
}
