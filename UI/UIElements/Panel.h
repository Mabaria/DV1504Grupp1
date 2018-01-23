#pragma once

#include <d3d11.h>

class Panel 
{
public:

	Panel(int width, int height, int top, int left);
	~Panel();
	
	const void SetWidth(int width);
	const int GetWidth() const;

	const void SetHeight(int height);
	const int GetHeight() const;

	const void SetTop(int top);
	const int GetTop() const;

	const void SetLeft(int left);
	const int GetLeft() const;

	const bool CreateVertexBuffer(float *vertices);
	const bool CreateIndexBuffer(int *indices);


private:

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
	ID3D11Texture2D *mTexture; // DDSTextureLoader?
	ID3D11ShaderResourceView *mShaderResourceView;
	ID3D11Buffer *mVertexBuffer;
	ID3D11Buffer *mIndexBuffer;
	int mNumberOfIndices;
	// ----------------------------------------------
};
