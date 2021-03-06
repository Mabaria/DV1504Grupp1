#include "Panel3D.h"

#include <iostream>

Panel3D::Panel3D(int width, int height, int top, int left,
	HWND handle, LPCTSTR title, bool movableCamera)
	:Panel(width, height, top, left, handle, title), mDirect3D(width, height)
{
	this->mDirect3D.Init(this->mPanelWindow);
	this->mDirect2D->InitDeviceAndContext(this->mDirect3D.GetDXGIDevice());
	
	// Binding the back buffer from the swap chain
	// to the direct2d 
	this->mpBackBuffer = nullptr;
	this->mDirect3D.GetSwapChain()->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D), 
		(void**)&this->mpBackBuffer);
	this->BindTextureToBitmap(this->mpBackBuffer);

	// bfcull test
	//D3D11_RASTERIZER_DESC rast_desc{};
	//rast_desc.CullMode = D3D11_CULL_NONE;
	//rast_desc.FillMode = D3D11_FILL_SOLID;
	//ID3D11RasterizerState *rs_state = nullptr;
	//this->mDirect3D.GetDevice()->CreateRasterizerState(&rast_desc, &rs_state);
	//this->mDirect3D.GetContext()->RSSetState(rs_state);
	//rs_state->Release();
	// ----------

	this->mpVertexShader = nullptr;
	this->mpGeometryShader = nullptr;
	this->mpPixelShader = nullptr;
	this->mpInputLayout = nullptr;

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

	this->mpCamera = nullptr;
	this->mpViewBuffer = nullptr;
	this->mpProjBuffer = nullptr;


	this->mMovableCamera = false;
	this->mShowCursor = true;
	this->mMovableCamera = movableCamera;

	this->mpActions = nullptr;

	this->mpIconBitmap = nullptr;
	this->mpNumberBitmap = nullptr;

	// Puts the ghost action feature in a zero state.
	this->ResetIcon();

	this->mUsingGhosts = false;
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

	for (size_t i = 0; i < this->mpPixelShaders.size(); i++)
	{
		if (this->mpPixelShaders[i])
		{
			this->mpPixelShaders[i]->Release();
			this->mpPixelShaders[i] = nullptr;
		}
	}

	if (this->mpMovableCameraComponent)
	{
		delete this->mpMovableCameraComponent;
		this->mpMovableCameraComponent = nullptr;
	}
	if (this->mpActions != nullptr)
	{
		delete this->mpActions;
	}
	if (this->mpIconBitmap)
	{
		this->mpIconBitmap->Release();
		this->mpIconBitmap = nullptr;
	}
	if (this->mpNumberBitmap)
	{			
		this->mpNumberBitmap->Release();
		this->mpNumberBitmap = nullptr;
	}

	if (this->mpBackBuffer)
	{
		this->mpBackBuffer->Release();
		this->mpBackBuffer = nullptr;
	}
	
}

D3D11 & Panel3D::rGetDirect3D()
{
	return this->mDirect3D;
}

// Pixel Shader ID is based on the position in the array it has.
// Which is in order of time it was added. 

const void Panel3D::AddMeshObject(
	std::string name,
	std::vector<std::vector<unsigned int>> indices,
	std::vector<std::vector<Vertex>> vertices,
	int pixelShaderID,
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

		HoverData hover_data = { false };
		event_desc.ByteWidth = sizeof(HoverData);
		event_data.pSysMem = &hover_data;

		for (int i = 0; i < 20; i++)
		{
			this->mDirect3D.GetDevice()->CreateBuffer(
				&event_desc,
				&event_data,
				this->mpMeshObjects.back()->rGetHoverBuffer(i)
			);
		}
	}

	// If pixel shader ID doesn't exist change it to default
	if (pixelShaderID >= (int)this->mpPixelShaders.size())
		pixelShaderID = 0;

	this->mpMeshObjects.back()->SetPixelShaderID(pixelShaderID);
}

// Pixel Shader ID is based on the position in the array it has.
// Which is in order of time it was added. 

const void Panel3D::AddMeshObject(MeshObject * meshObject,
	int pixelShaderID,
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
		event_desc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
		event_desc.ByteWidth		= sizeof(EventData);
		event_desc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
		event_desc.Usage			= D3D11_USAGE_DYNAMIC;

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

		HoverData hover_data = { false };
		event_desc.ByteWidth = sizeof(HoverData);
		event_data.pSysMem = &hover_data;

		for (int i = 0; i < 20; i++)
		{
			this->mDirect3D.GetDevice()->CreateBuffer(
				&event_desc,
				&event_data,
				this->mpMeshObjects.back()->rGetHoverBuffer(i)
			);
		}
	}

	// If pixel shader ID doesn't exist change it to default
	if (pixelShaderID >= (int)this->mpPixelShaders.size())
		pixelShaderID = 0;

	this->mpMeshObjects.back()->SetPixelShaderID(pixelShaderID);
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
		sizeof(mInputDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC),
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

	this->mpPixelShaders.push_back(this->mpPixelShader);

	// Setting up input assembler.
	this->mDirect3D.GetContext()->IASetPrimitiveTopology
	(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->mDirect3D.GetContext()->IASetInputLayout(this->mpInputLayout);

	this->AddPixelShader(L"../../GraphicsEngine/PS_Boat.hlsl");
	this->AddPixelShader(L"../../GraphicsEngine/PS_Text3D.hlsl");
	this->AddPixelShader(L"../../GraphicsEngine/PS_EventBox.hlsl");

	return result;
}

bool Panel3D::AddPixelShader(LPCWSTR pixelShaderPath)
{
	// Create pixel shader
	bool result = this->mDirect3D.CreateShaders(
		L"",
		L"",
		pixelShaderPath,
		nullptr,
		nullptr,
		&this->mpPixelShader,
		nullptr,
		0,
		nullptr);

	// If successful then add it to the rest
	if(result)
		this->mpPixelShaders.push_back(this->mpPixelShader);

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
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth = (UINT)(sizeof(unsigned int) * indices.size());
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

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
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.ByteWidth = sizeof(XMMATRIX);

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
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.ByteWidth = sizeof(MaterialStruct);

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

const void Panel3D::mUpdateGhostTransform()
{
	// Calculating the center of the ghost for scaling and rotation.
	float icon_width  = this->mGhostIconRect.right - this->mGhostIconRect.left;
	float icon_height = this->mGhostIconRect.bottom - this->mGhostIconRect.top;

	XMVECTOR ghost_center = 
	{
		this->mGhostPosition.left + icon_width / 2.0f,
		this->mGhostPosition.top + icon_height / 2.0f,
		0.0f,
		0.0f 
	};

	// Creating a 2D transform XMMATRIX, storing that in an XMFLOAT4X4.
	XMFLOAT4X4 transform;
	DirectX::XMStoreFloat4x4(&transform, XMMatrixTransformation2D(
		ghost_center,
		0.0f,
		{ this->mGhostScale, this->mGhostScale, 0.0f, 0.0f },
		ghost_center,
		this->mGhostRotation * XM_PI / 2.0f,
		{ 0.0f, 0.0f, 0.0f, 0.0f }));

	// Element-wise copying every slot in the FLOAT4X4
	// to the D2D1_MATRIX_4X4_F that the draw call requires.
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->mGhostTransform.m[i][j] = transform.m[i][j];
		}
	}
}

const void Panel3D::mUpdateGhosts()
{
	if (this->mGhostActive && this->IsMouseInsidePanel())
	{

		// Calculating the position of the ghost
		// with the cursor position as the center.
		Position mouse_pos = Mouse::GetPosition();

		float icon_width = this->mGhostIconRect.right - this->mGhostIconRect.left;
		float icon_height = this->mGhostIconRect.bottom - this->mGhostIconRect.top;

		this->mGhostPosition.left = mouse_pos.x - icon_width / 2.0f;
		this->mGhostPosition.top = mouse_pos.y - icon_height / 2.0f;
		this->mGhostPosition.right = this->mGhostPosition.left + icon_width;
		this->mGhostPosition.bottom = this->mGhostPosition.top + icon_height;

		// ----Super cool scaling----
		static float max_zoom_in = 0.5f;
		static float max_zoom_out = 0.15f;

		static float zoom_increment = (max_zoom_in - max_zoom_out) / 10.0f;

		float scroll = Mouse::GetScroll();
		if (scroll != 0.0f)
		{
			this->mGhostScale += scroll * zoom_increment;

			if (this->mGhostScale > max_zoom_in)
			{
				this->mGhostScale = max_zoom_in;
			}
			else if (this->mGhostScale < max_zoom_out)
			{
				this->mGhostScale = max_zoom_out;
			}
		}
		// --------------------------

		this->mUpdateGhostTransform();
	}
}

const void Panel3D::mDrawGhosts()
{
	static float ghost_opacity = 0.4f;
	if (this->mGhostActive && this->IsMouseInsidePanel())
	{
		this->DrawBitmapToTexture(
			this->GetBitmapByName("iconBitmap"),
			this->mGhostPosition,
			this->mGhostIconRect,
			ghost_opacity,
			&this->mGhostTransform);

		this->DrawBitmapToTexture(
			this->GetBitmapByName("numberBitmap"),
			this->mGhostPosition,
			this->mGhostNumberRect,
			ghost_opacity,
			&this->mGhostTransform);
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
	if(this->mpActions != nullptr)
		this->mpActions->SetCamera(camera);

	if (!this->mpViewBuffer)
	{
		this->mCreateMatrixBuffer(
			&this->mpCamera->GetViewMatrix(),
			&this->mpViewBuffer);
	}
	if (!this->mpProjBuffer)
	{
		this->mCreateMatrixBuffer(
			&this->mpCamera->GetProjectionMatrix(),
			&this->mpProjBuffer);
	}

	if (this->mpMovableCameraComponent)
	{
		delete this->mpMovableCameraComponent;
	}

	this->mpMovableCameraComponent = new MovableCameraComponent();
	this->mpMovableCameraComponent->Initialize(*this->mpCamera);
	this->mMovableCamera = true;
	if (this->mpActions != nullptr)
		this->mpActions->SetMoveableCamera(this->mpMovableCameraComponent);
}

void Panel3D::Update(Button * attribute)
{
	this->mpCamera->Update(attribute);
}

Camera * Panel3D::GetActiveCamera()
{
	return this->mpCamera;
}

void Panel3D::BindTextureToBitmap(ID3D11Texture2D * texture)
{
	IDXGISurface *surface;
	ID2D1Bitmap1 *bitmap;

	texture->QueryInterface(&surface);

	this->mDirect2D->GetpContext()->CreateBitmapFromDxgiSurface(
		surface,
		nullptr,
		&bitmap
	);

	this->mDirect2D->GetpContext()->SetTarget(bitmap);
	bitmap->Release();
	surface->Release();
}

void Panel3D::DrawBitmapToTexture(
	ID2D1Bitmap *bitmap,
	float startX,
	float startY,
	float widthOfTex,
	float heightOfTex)
{
	this->mDirect2D->GetpContext()->BeginDraw();
	this->mDirect2D->GetpContext()->DrawBitmap(
		bitmap,
		D2D1::RectF(startX, startY, widthOfTex, heightOfTex),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(
			0,
			0,
			bitmap->GetSize().width,
			bitmap->GetSize().height)
	);

	this->mDirect2D->GetpContext()->EndDraw();

}

void Panel3D::DrawBitmapToTexture(ID2D1Bitmap * bitmap, 
	D2D1_RECT_F destRect, 
	const D2D1_RECT_F sourceRect, 
	FLOAT opacity, 
	const D2D1_MATRIX_4X4_F *transform)
{
	this->mDirect2D->GetpContext()->BeginDraw();
	this->mDirect2D->GetpContext()->DrawBitmap(
		bitmap,
		&destRect,
		opacity,
		D2D1_INTERPOLATION_MODE_LINEAR,
		&sourceRect,
		transform);
	
	this->mDirect2D->GetpContext()->EndDraw();
}

void Panel3D::AddAction(float x, float y, ActionData data)
{
	this->mpActions->AddAction(x, y, data);
}

void Panel3D::InitActions()
{
	if (this->mpActions == nullptr)
	{
		this->mpActions = new Actions();
		this->mpActions->Init(&this->mDirect3D);	
		
		this->LoadImageToBitmap("../../Models/Symbols.dds", "iconBitmap");
		this->LoadImageToBitmap("../../Models/Numbers.dds", "numberBitmap");
		this->mUsingGhosts = true;
	}
}

Actions * Panel3D::pGetActions()
{
	return this->mpActions;
}

const void Panel3D::SetActionHover(bool state)
{
	this->mGhostActive = state;
}

const void Panel3D::SetIcon(uint32_t data)
{
	// Early exit if there was no action in data.
	if (data == No_Action)
	{
		return;
	}

	// Using magic to extract data from the uint32_t.
	int rotation_index	= (data >> 4) & 7;
	int icon_index		= data & 15;
	int number_index	= (data >> 9) & 15;
	bool has_number		= number_index == 0 ? false : true;

	this->mGhostStationary = rotation_index == 4 ? true : false;

	// Sizes of the entire bitmap.
	static D2D1_SIZE_F icon_bitmap_size	= 
		this->GetBitmapByName("iconBitmap")->GetSize();
	static D2D1_SIZE_F number_bitmap_size = 
		this->GetBitmapByName("numberBitmap")->GetSize();

	// Sizes of individual icons/numbers.
	static D2D1_SIZE_F icon_size = { 
		icon_bitmap_size.width / 4, 
		icon_bitmap_size.height / 4 };
	static D2D1_SIZE_F number_size = { 
		number_bitmap_size.width / 3, 
		number_bitmap_size.height / 3 };
	
	// Calculating the rectangle in the bitmap which holds the icon.
	this->mGhostIconRect.top	= (icon_index / 4) * icon_size.height;
	this->mGhostIconRect.left	= (icon_index % 4) * icon_size.width;
	this->mGhostIconRect.bottom = this->mGhostIconRect.top	+ icon_size.height;
	this->mGhostIconRect.right	= this->mGhostIconRect.left + icon_size.width;

	// Initializing a number rect to not draw anything from the 
	// number bitmap if there is no number.
	D2D1_RECT_F number = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	if (has_number)
	{
		number_index--;
		number.top		= (number_index / 3) * number_size.height;
		number.left		= (number_index % 3) * number_size.width;
		number.bottom	= number.top + number_size.height;
		number.right	= number.left + number_size.width;
	}
	this->mGhostNumberRect = number;	

	this->mGhostScale = 0.5f;
}

const void Panel3D::RotateIcon()
{
	// Increments current rotation and takes it modulo 4
	// for the 4 possible stages of rotation.
	if (!this->mGhostStationary)
	{
		this->mGhostRotation++;
		this->mGhostRotation %= 4;
	}
}

const void Panel3D::ResetIcon()
{
	this->mGhostActive		= false;
	this->mGhostStationary	= false;
	this->mGhostRotation	= 0;
	this->mGhostScale		= 0.0f;
	this->mGhostNumberRect	= { 0 };
	this->mGhostIconRect	= { 0 };
	this->mGhostTransform	= { 0 };
	this->mGhostPosition	= { 0 };
}

MovableCameraComponent * Panel3D::GetMovableComponent()
{
	return this->mpMovableCameraComponent;
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
			show_cursor = this->mpMovableCameraComponent->Update();
		}
	}

	// Updates animation if necessary
	this->mpMovableCameraComponent->UpdateAnimation();

	if (this->mShowCursor != show_cursor)
	{
		this->mShowCursor = show_cursor;
		ShowCursor(this->mShowCursor);
	}
	if (this->mUsingGhosts)
	{
		this->mUpdateGhosts();
	}
}

const void Panel3D::Draw()
{
	this->mDirect3D.Clear();
	//!!! I ADDED THIS
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

	// Stride (vertex size) and offset are
	// declared because they have to be referenced.
	UINT stride = (UINT)sizeof(Vertex);
	UINT offset = 0;

	// For readability.
	ID3D11PixelShader *pixel_shader = nullptr;
	ID3D11Buffer *vertex_buffer		= nullptr;
	ID3D11Buffer *index_buffer		= nullptr;
	ID3D11Buffer *matrix_buffer		= nullptr;
	ID3D11Buffer *material_buffer	= nullptr;
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
		// ___ SET PIXEL SHADER ___
		ID3D11PixelShader* temp = this->mpPixelShaders[
			this->mpMeshObjects[i]->GetPixelShaderID()
		];
		if (pixel_shader != temp)
		{
			pixel_shader = temp;
			this->mDirect3D.GetContext()->PSSetShader(pixel_shader, nullptr, 0);
		}
		// ___ END ___

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
			index_buffer = *this->mpMeshObjects[i]->pGetIndexBuffer(j);
			vertex_buffer = *this->mpMeshObjects[i]->pGetVertexBuffer(j);
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

			this->mDirect3D.GetContext()->PSSetConstantBuffers(
				2, 1, this->mpMeshObjects[i]->rGetHoverBuffer(j)
			);



			numIndices = (UINT)this->mpMeshObjects[i]->GetIndices()[j].size();
			this->mDirect3D.GetContext()->DrawIndexed(
				numIndices,	// Number of indices.
				0,			// Start index location.
				offset);	// Base vertex location.
		}
	}

	// Clear the depth buffer and draw the actions
	this->mDirect3D.ClearDepth();
	if (this->mpActions != nullptr)
	{
		this->mpActions->Draw();
	}

	if (this->mUsingGhosts)
	{
		this->mDrawGhosts();
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
