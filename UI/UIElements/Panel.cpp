//#include "stdafx.h"
#include "Panel.h"

Panel::Panel(int width, int height, int top, int left, ID3D11Device *device)
{
	this->mDevice = device;

	this->mWidth  = width;
	this->mHeight = height;
	this->mTop	  = top;
	this->mLeft	  = left;

	this->mViewport.Width	 = this->mWidth;
	this->mViewport.Height	 = this->mHeight;
	this->mViewport.TopLeftY = this->mTop;
	this->mViewport.TopLeftX = this->mLeft;
	this->mViewport.MinDepth = 0.0f;
	this->mViewport.MaxDepth = 1.0f;

	this->mTexture			  = nullptr;
	this->mShaderResourceView = nullptr;
	this->mVertexBuffer		  = nullptr;
	this->mIndexBuffer		  = nullptr;

	D3D11_TEXTURE2D_DESC renderTargetDesc{};
	renderTargetDesc.Width			  = this->mWidth;
	renderTargetDesc.Height			  = this->mHeight;
	renderTargetDesc.MipLevels		  = 1;
	renderTargetDesc.ArraySize		  = 1;
	renderTargetDesc.Format			  = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTargetDesc.SampleDesc.Count = 1;
	renderTargetDesc.Usage			  = D3D11_USAGE_DEFAULT;
	renderTargetDesc.BindFlags		  = D3D11_BIND_RENDER_TARGET;
	renderTargetDesc.CPUAccessFlags	  = 0;
	renderTargetDesc.MiscFlags		  = 0;

	if (FAILED(this->mDevice->CreateTexture2D(&renderTargetDesc, nullptr, &this->mRenderTarget)))
	{
		MessageBoxA(NULL, "Error creating render target texture for panel.", NULL, MB_OK);
		exit(-1);
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	renderTargetViewDesc.Format				= renderTargetDesc.Format;
	renderTargetViewDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	
	if (FAILED(this->mDevice->CreateRenderTargetView(this->mRenderTarget, &renderTargetViewDesc, &this->mRenderTargetView)))
	{
		MessageBoxA(NULL, "Error creating render target view for panel.", NULL, MB_OK);
		exit(-1);
	}
}

Panel::~Panel()
{
	if (this->mRenderTarget != nullptr)
	{
		this->mRenderTarget->Release();
	}
	if (this->mRenderTargetView != nullptr) 
	{
		this->mRenderTargetView->Release();
	}
	if (this->mTexture != nullptr)
	{
		this->mTexture->Release();
	}
	if (this->mShaderResourceView != nullptr)
	{
		this->mShaderResourceView->Release();
	}
	if (this->mVertexBuffer != nullptr)
	{
		this->mVertexBuffer->Release();
	}
	if (this->mIndexBuffer != nullptr)
	{
		this->mIndexBuffer->Release();
	}
}

const void Panel::SetWidth(int width)
{
	this->mWidth = width;
}

const int Panel::GetWidth() const
{
	return this->mWidth;
}

const void Panel::SetHeight(int height)
{
	this->mHeight = height;
}

const int Panel::GetHeight() const
{
	return this->mHeight;
}

const void Panel::SetTop(int top)
{
	this->mTop = top;
}

const int Panel::GetTop() const
{
	return this->mTop;
}

const void Panel::SetLeft(int left)
{
	this->mLeft = left;
}

const int Panel::GetLeft() const
{
	return this->mLeft;
}

const void Panel::CreateVertexBuffer(float * vertices)
{
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage	   = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = (UINT)(sizeof(*vertices));

	D3D11_SUBRESOURCE_DATA vertexData{};
	vertexData.pSysMem = (void*)vertices;

	if (FAILED(this->mDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &this->mVertexBuffer)))
	{
		MessageBoxA(NULL, "Error creating vertex buffer for panel.", NULL, MB_OK);
		exit(-1);
	}
}

const void Panel::CreateIndexBuffer(int * indices, int numberOfIndices)
{
	this->mNumberOfIndices = numberOfIndices;

	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = (UINT)(sizeof(*indices));

	D3D11_SUBRESOURCE_DATA indexData{};
	indexData.pSysMem = (void*)indices;

	if (FAILED(this->mDevice->CreateBuffer(&indexBufferDesc, &indexData, &this->mIndexBuffer)))
	{
		MessageBoxA(NULL, "Error creating index buffer for panel.", NULL, MB_OK);
		exit(-1);
	}
}

ID3D11RenderTargetView * Panel::rGetRenderTargetView() const
{
	return this->mRenderTargetView;
}

const ID3D11ShaderResourceView * Panel::rGetShaderResourceView() const
{
	return this->mShaderResourceView;
}

const ID3D11Buffer * Panel::GetVertexBuffer() const
{
	return this->mVertexBuffer;
}

const ID3D11Buffer * Panel::GetIndexBuffer() const
{
	return this->mIndexBuffer;
}

const int Panel::GetNumberOfIndices() const
{
	return this->mNumberOfIndices;
}