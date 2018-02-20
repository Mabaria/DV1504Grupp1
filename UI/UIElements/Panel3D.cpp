#include "Panel3D.h"

Panel3D::Panel3D(int width, int height, int top, int left,
	HWND handle, LPCTSTR title, bool movableCamera)
	:Panel(width, height, top, left, handle, title), mDirect3D(width, height)
{
	this->mDirect3D.Init(this->mPanelWindow);

	// bfcull test
	//D3D11_RASTERIZER_DESC rast_desc{};
	//rast_desc.CullMode = D3D11_CULL_NONE;
	//rast_desc.FillMode = D3D11_FILL_SOLID;
	//ID3D11RasterizerState *rs_state = nullptr;
	//this->mDirect3D.GetDevice()->CreateRasterizerState(&rast_desc, &rs_state);
	//this->mDirect3D.GetContext()->RSSetState(rs_state);
	//rs_state->Release();
	// ----------

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


	this->mShowCursor	= true;
	this->mRadius		= 10.0f;
	this->mDirection	= DirectX::XMVector3Normalize({ 0.5f, 0.3f, 0.5f });
	this->mSpeed		= 0.06f;
	this->mMouseDiff.x	= 0.0f;
	this->mMouseDiff.y	= 0.0f;
	this->mBtnToPan		= Buttons::Right;

	this->mOrthographicMaxView = 2.0f;
	this->mMovableCamera = movableCamera;
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

const void Panel3D::AddMeshObject(
	std::string name,
	std::vector<std::vector<unsigned int>> indices,
	std::vector<std::vector<Vertex>> vertices,
	std::wstring texturePath,
	bool use_event)
{
	MeshObject *mesh_object = new MeshObject(name, indices, vertices);
	this->mpMeshObjects.push_back(mesh_object);

	for (int i = 0; i < this->mpMeshObjects.back()->GetNumberOfIndexBuffers(); i++)
	{
		this->CreateIndexBuffer(indices[i]);
		this->CreateVertexBuffer(vertices[i]);
	}
	this->mCreateMatrixBuffer(
		this->mpMeshObjects.back()->rGetModelMatrix(),
		this->mpMeshObjects.back()->rGetMatrixBuffer());

	for (int i = 0; i < this->mpMeshObjects.back()->GetNumberOfMaterialBuffers(); i++)
	{
		this->mCreateMaterialBuffer(
			&mesh_object->pGetMaterialHandler()->GetMaterialStruct(i));
	}

	if (this->mpMeshObjects.back()->GetNumberOfMaterialBuffers() == 0)
	{
		// Add a default material if no material exists
		MaterialStruct defaultMat =
		{ 0.5f, 0.5f, 0.5f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,
			1.0f };
		this->mCreateMaterialBuffer(&defaultMat);


	}


	if (texturePath != L"")
	{
		this->CreateTexture(texturePath);
	}

	if (use_event)
	{
		EventData data = { 0 };

		D3D11_BUFFER_DESC event_desc = { 0 };
		event_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		event_desc.ByteWidth = sizeof(EventData);
		event_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		event_desc.Usage = D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA event_data = { 0 };
		event_data.pSysMem = &data;

		for (int i = 0; i < 20; i++)
		{
			this->mDirect3D.GetDevice()->CreateBuffer(
				&event_desc,
				&event_data,
				this->mpMeshObjects.back()->rGetEventBuffer(i)
			);
		}
	}
}

const void Panel3D::AddMeshObject(MeshObject * meshObject,
	std::wstring texturePath,
	bool use_event)
{
	this->mpMeshObjects.push_back(new MeshObject(*meshObject));

	for (int i = 0; i < this->mpMeshObjects.back()->GetNumberOfIndexBuffers(); i++)
	{
		this->CreateIndexBuffer(meshObject->GetIndices()[i]);
		this->CreateVertexBuffer(meshObject->GetVertices()[i]);
	}

	this->mCreateMatrixBuffer(
		this->mpMeshObjects.back()->rGetModelMatrix(),
		this->mpMeshObjects.back()->rGetMatrixBuffer());

	for (int i = 0; i < this->mpMeshObjects.back()->GetNumberOfMaterialBuffers(); i++)
	{
		this->mCreateMaterialBuffer(
			&meshObject->pGetMaterialHandler()->GetMaterialStruct(i));
	}

	if (this->mpMeshObjects.back()->GetNumberOfMaterialBuffers() == 0)
	{
		// Add a default material if no material exists
		MaterialStruct defaultMat =
		{ 1.0f, 1.0f, 1.0f,
		  1.0f, 1.0f, 1.0f,
		  1.0f, 1.0f, 1.0f,
		  1.0f,
		  1.0f };
		this->mCreateMaterialBuffer(&defaultMat);
			
		
	}

		if (texturePath != L"")
		{
			this->CreateTexture(texturePath);
		}
	
		if (use_event)
		{
			EventData data = { 0 };

			D3D11_BUFFER_DESC event_desc = { 0 };
			event_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			event_desc.ByteWidth = sizeof(EventData);
			event_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			event_desc.Usage = D3D11_USAGE_DYNAMIC;

			D3D11_SUBRESOURCE_DATA event_data = { 0 };
			event_data.pSysMem = &data;

			for (int i = 0; i < 20; i++)
			{
				this->mDirect3D.GetDevice()->CreateBuffer(
					&event_desc,
					&event_data,
					this->mpMeshObjects.back()->rGetEventBuffer(i)
				);
			}
		}
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

const void Panel3D::mCreateMatrixBuffer(
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
		MessageBoxA(NULL, "Error creating matrix buffer.", NULL, MB_OK);
		exit(-1);
	}
}

const void Panel3D::mCreateMaterialBuffer(
	MaterialStruct * matStruct)
{
	ID3D11Buffer *material_buffer = nullptr;
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage			= D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	buffer_desc.ByteWidth		= sizeof(MaterialStruct);

	D3D11_SUBRESOURCE_DATA buffer_data{};
	buffer_data.pSysMem = matStruct;

	// Create buffer
	if (FAILED(this->mDirect3D.GetDevice()->CreateBuffer(
		&buffer_desc,
		&buffer_data,
		&material_buffer)))
	{
		MessageBoxA(NULL, "Error creating material buffer.", NULL, MB_OK);
		exit(-1);
	}
	this->mpMeshObjects.back()->AddMaterialBuffer(&material_buffer);
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

const void Panel3D::UpdateMatrixBuffer(std::string name)
{
	// Getting the mesh object of the given name.
	MeshObject *mesh_object = this->rGetMeshObject(name);
	
	if (mesh_object)
	{
		// Getting the constant buffer and model matrix from that mesh object.
		ID3D11Buffer *matrix_buffer = *mesh_object->rGetMatrixBuffer();
		const XMMATRIX *model_matrix = mesh_object->rGetModelMatrix();

		// Mapping the buffer data to the CPU in order to change it,
		// then unmapping it again, which updates the buffer contents.
		D3D11_MAPPED_SUBRESOURCE data_ptr;
		this->mDirect3D.GetContext()->Map(
			matrix_buffer,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&data_ptr);

		memcpy(data_ptr.pData, model_matrix, sizeof(XMMATRIX));
		this->mDirect3D.GetContext()->Unmap(matrix_buffer, 0);
	}
}

const void Panel3D::UpdateMatrixBuffer(
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

const void Panel3D::UpdateMatrixBuffer(int index)
{
	if (index > -1 && index < (int)this->mpMeshObjects.size())
	{
		// Getting the mesh object of the given name.
		MeshObject *mesh_object = this->mpMeshObjects[index];

		// Getting the constant buffer and model matrix from that mesh object.
		ID3D11Buffer *matrix_buffer = *mesh_object->rGetMatrixBuffer();
		const XMMATRIX *model_matrix = mesh_object->rGetModelMatrix();

		// Mapping the buffer data to the CPU in order to change it,
		// then unmapping it again, which updates the buffer contents.
		D3D11_MAPPED_SUBRESOURCE data_ptr{};
		this->mDirect3D.GetContext()->Map(
			matrix_buffer,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&data_ptr);

		memcpy(data_ptr.pData, model_matrix, sizeof(XMMATRIX));
		this->mDirect3D.GetContext()->Unmap(matrix_buffer, 0);
	}
}

const void Panel3D::SetCamera(Camera * camera)
{
	this->mpCamera = camera;

	this->mCreateMatrixBuffer(
		&this->mpCamera->GetViewMatrix(), 
		&this->mpViewBuffer);
	this->mCreateMatrixBuffer(
		&this->mpCamera->GetProjectionMatrix(), 
		&this->mpProjBuffer);

	this->mDirection = 
		this->mpCamera->GetPosition() - this->mpCamera->GetLookVector();

	this->mRadius = XMVectorGetX(XMVector3Length(this->mDirection));
	this->mDirection = XMVector3Normalize(this->mDirection);

	//this->UpdateCamera();
}

void Panel3D::Update(Button * attribute)
{
	if (attribute->GetName().compare("Reset") == 0)
	{
		this->mRadius = 4.0f;
		this->mDirection = DirectX::XMVector3Normalize({
			-0.025140788f,
			1.28821635f,
			3.78684092f });
	}
	else
	{
		this->mRadius = 6.0f;
		this->mDirection = DirectX::XMVector3Normalize({
			-0.02f,
			6.19f,
			2.99f });
	}
	this->mMouseDiff.x = 0.0f;
	this->mMouseDiff.y = 0.0f;
	this->UpdateCamera();
}

const void Panel3D::Update()
{
	bool show_cursor = true;

	// Updating the constant buffers of the mesh objects.
	for (int i = 0; i < (int)this->mpMeshObjects.size(); i++)
	{
		UpdateMatrixBuffer(i);
	}
	if (this->mpCamera)
	{
		// Updating the camera buffers.
		UpdateMatrixBuffer(
			&this->mpCamera->GetViewMatrix(),
			&this->mpViewBuffer);
		UpdateMatrixBuffer(
			&this->mpCamera->GetProjectionMatrix(),
			&this->mpProjBuffer);
	}
	this->UpdateWindowSize();

	if (this->IsMouseInsidePanel())
	{
		
		if (this->mMovableCamera)
		{
			this->UpdateMouse();
			show_cursor = this->mpCamera ? this->UpdateCamera() : true;
		}
	}

	if (this->mShowCursor != show_cursor)
	{
		this->mShowCursor = show_cursor;
		ShowCursor(this->mShowCursor);
	}
}

const void Panel3D::UpdateMouse()
{
	if (Mouse::IsButtonPressed(this->mBtnToPan))
	{
		POINT mouse_pos;
		GetCursorPos(&mouse_pos);
		this->mMouseOrigin.x = (float)mouse_pos.x;
		this->mMouseOrigin.y = (float)mouse_pos.y;
	}

	if (Mouse::IsButtonDown(this->mBtnToPan))
	{
		POINT mouse_pos;
		GetCursorPos(&mouse_pos);

		this->mMouseDiff.x = mouse_pos.x - this->mMouseOrigin.x;
		this->mMouseDiff.y = mouse_pos.y - this->mMouseOrigin.y;

		SetCursorPos(
			(int)this->mMouseOrigin.x,
			(int)this->mMouseOrigin.y
		);

		int dead_zone = 1;

		if (abs(this->mMouseDiff.x) < dead_zone)
			this->mMouseDiff.x = 0;
		if (abs(this->mMouseDiff.y) < dead_zone)
			this->mMouseDiff.y = 0;
	}
}

const bool Panel3D::UpdateCamera()
{
	bool flag = false;
	float scroll = Mouse::GetScroll();
	
	switch (this->mpCamera->GetProjectionMode())
	{
	case ORTHOGRAPHIC:
	{
		// Scroll - Zoom
		if (scroll != 0)
		{
			this->mpCamera->SetViewWidth(
				this->mpCamera->GetViewWidth() - (this->mSpeed * scroll)
			);

			this->mpCamera->SetViewHeight(
				this->mpCamera->GetViewHeight() - (this->mSpeed * scroll)
			);
		}

		// Restrict zoom out
		if (this->mpCamera->GetViewWidth() > this->mOrthographicMaxView)
			this->mpCamera->SetViewWidth(this->mOrthographicMaxView);

		if (this->mpCamera->GetViewHeight() > this->mOrthographicMaxView)
			this->mpCamera->SetViewHeight(this->mOrthographicMaxView);

		// Mouse movement - Pan
		if (Mouse::IsButtonDown(this->mBtnToPan))
		{
			flag = true;
			this->mpCamera->MoveCamera(
				{ this->mMouseDiff.x * -0.008f, this->mMouseDiff.y }, 
				this->mSpeed
			);
		}
		break;
	}

	case PERSPECTIVE:
	{
		// Scroll - Zoom
		if (scroll != 0)
		{
			this->mRadius -= this->mSpeed * scroll * 4.0f;

			if (this->mRadius < 1.0f)
				this->mRadius = 1.0f;

			if (this->mRadius > this->mpCamera->GetFarZ() - 1.0f)
				this->mRadius = this->mpCamera->GetFarZ() - 1.0f;
		}

		// Mouse movement - Rotate
		if (Mouse::IsButtonDown(this->mBtnToPan))
		{
			flag = true;
			DirectX::XMVECTOR right = DirectX::XMVector3Cross(
				this->mpCamera->GetLookVector() - this->mpCamera->GetPosition(),
				this->mpCamera->GetUpVector()
			);

			DirectX::XMVECTOR up = DirectX::XMVector3Cross(
				this->mpCamera->GetLookVector() - this->mpCamera->GetPosition(),
				right
			);

			DirectX::XMVECTOR newPos = this->mpCamera->GetPosition()
				+ up * this->mMouseDiff.y * -0.0008f
				+ right * this->mMouseDiff.x * 0.005f;

			DirectX::XMVECTOR dir = DirectX::XMVector3Normalize(
				newPos - this->mpCamera->GetLookVector()
			);

			float y = DirectX::XMVectorGetY(dir);
			if (y < 0.0f)
			{
				y = 0.0f;
				dir = DirectX::XMVectorSetY(dir, y);
				dir = DirectX::XMVector3Normalize(dir);
			}
			else if (y > 0.9f)
			{
				y = 0.9f;
				dir = DirectX::XMVectorSetY(dir, y);
				dir = DirectX::XMVector3Normalize(dir);
			}

			this->mDirection = dir;
		}
		// Update Camera

		//this->mpCamera->SetCameraPosition(this->mpCamera->GetLookVector());
		this->mpCamera->SetCameraPosition(0.0f, 0.0f, 0.0f);
		this->mpCamera->MoveCamera(this->mDirection, this->mRadius);
		break;
	}

	default:
		break;

	}

	return !flag; // If mouse is down then don't show cursor
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
	ID3D11Buffer* matrix_buffer	= nullptr;
	ID3D11Buffer* material_buffer = nullptr;
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


		matrix_buffer = *this->mpMeshObjects[i]->rGetMatrixBuffer();
		this->mDirect3D.GetContext()->PSSetShaderResources(
			0, 1, this->mpMeshObjects[i]->rGetTextureView()
		);


		// Setting the constant buffer to the vertex shader.
		this->mDirect3D.GetContext()->VSSetConstantBuffers(
			0,					// Start slot.
			1,					// Number of buffers
			&matrix_buffer);	// Constant buffer.

		for (int j = 0; j < this->mpMeshObjects[i]->GetNumberOfIndexBuffers(); j++)
		{
			index_buffer	= *this->mpMeshObjects[i]->pGetIndexBuffer(j);
			vertex_buffer	= *this->mpMeshObjects[i]->pGetVertexBuffer(j);
			material_buffer = *this->mpMeshObjects[i]->rGetMaterialBuffer(
				this->mpMeshObjects[i]->GetMaterialIndexForIndexBuffer(j));

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

			this->mDirect3D.GetContext()->PSSetConstantBuffers(
				0, 1, this->mpMeshObjects[i]->rGetEventBuffer(j)
			);

			// Setting the material buffer to the pixel shader.
			this->mDirect3D.GetContext()->PSSetConstantBuffers(
				1,
				1,
				&material_buffer
			);



			numIndices = (UINT)this->mpMeshObjects[i]->GetIndices()[j].size();
			this->mDirect3D.GetContext()->DrawIndexed(
				numIndices,	// Number of indices.
				0,			// Start index location.
				offset);	// Base vertex location.
		}		
	}
	IDXGISurface *BackBuffer;
	this->mDirect3D.GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&BackBuffer));
	this->mDirect2D.GetpContext()->
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
