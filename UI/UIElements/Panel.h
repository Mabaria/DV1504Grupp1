#pragma once

#include <d3d11.h>

class Panel 
{
public:
	Panel(int width, int height, int top, int left, ID3D11Device *device);
	~Panel();
	
	const void SetWidth(int width);
	const int GetWidth() const;

	const void SetHeight(int height);
	const int GetHeight() const;

	const void SetTop(int top);
	const int GetTop() const;

	const void SetLeft(int left);
	const int GetLeft() const;

	const void CreateVertexBuffer(float *vertices);
	const void CreateIndexBuffer(int *indices, int numberOfIndices);

	ID3D11RenderTargetView *rGetRenderTargetView() const;
	const ID3D11ShaderResourceView *rGetShaderResourceView() const;

	const ID3D11Buffer *GetVertexBuffer() const;
	const ID3D11Buffer *GetIndexBuffer() const;
	const int GetNumberOfIndices() const;

private:
	// Pointer to main device until better solution.
	ID3D11Device *mDevice;
	// ---------------------------------------------

	// Render target for rendering to the panel.
	ID3D11Texture2D *mRenderTarget;
	ID3D11RenderTargetView *mRenderTargetView;
	// -----------------------------------------
	
	// Viewport with parameters.
	D3D11_VIEWPORT mViewport;
	int mWidth;
	int mHeight;
	int mTop;
	int mLeft;
	// -------------------------
	
	// May or may not be used depending on the panel.
	ID3D11Texture2D *mTexture; 
	ID3D11ShaderResourceView *mShaderResourceView;
	ID3D11Buffer *mVertexBuffer;
	ID3D11Buffer *mIndexBuffer;
	int mNumberOfIndices;
	// ----------------------------------------------
};