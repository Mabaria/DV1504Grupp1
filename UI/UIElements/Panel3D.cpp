#include "Panel3D.h"

Panel3D::Panel3D(int width, int height, int top, int left, HWND handle)
	:Panel(width, height, top, left, handle), mDirect3D(handle, width, height)
{
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

const bool Panel3D::AddMeshObject(std::vector<std::vector<unsigned int>> indices, std::vector<Vertex> vertices)
{
	bool result = false;
	MeshObject new_mesh_object = MeshObject(indices, vertices);
	// TODO: Create buffers for mesh object.

	this->mMeshObjects.push_back(new_mesh_object);


	return result;
}

const bool Panel3D::CreateVertexBuffer(std::vector<Vertex> vertices, ID3D11Buffer *vertexBuffer)
{
	bool result = true;

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

const bool Panel3D::CreateIndexBuffer(std::vector<unsigned int> indices, ID3D11Buffer * indexBuffer)
{
	bool result = true;

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

}
