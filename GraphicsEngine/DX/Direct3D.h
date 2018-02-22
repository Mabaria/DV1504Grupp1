#pragma once

#include "../Window.h"
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

/*

	-- Direct 3D 11 --

	API used for 3D rendering

*/

class D3D11
{
public:
	D3D11();
	D3D11(
		const int width, 
		const int height);
	~D3D11();

	void Init(HWND window);

	bool CreateShaders(
		LPCWSTR vertexShaderPath,
		LPCWSTR geometryShaderPath,
		LPCWSTR pixelShaderPath,
		ID3D11VertexShader **pVertexshader,
		ID3D11GeometryShader **pGeometryShader,
		ID3D11PixelShader **pPixelShader,
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[],
		UINT nrOfElements,
		ID3D11InputLayout **pInputLayout
	);

	void Clear();

	IDXGISwapChain* GetSwapChain() const;
	ID3D11DeviceContext* GetContext() const;
	ID3D11Device* GetDevice() const;

	const DXGI_SAMPLE_DESC& GetSampleDesc() const;

	void ChangeClearColor(const float r, const float g, const float b);

	void ChangeSize(const float width, const float height);
	void ChangeSize(const Size size);
	const Size& GetSize();

	void ReleaseCOM(IUnknown *object);
	IDXGIDevice *GetDXGIDevice();

private:
	ID3D11Device *mDevice;
	ID3D11DeviceContext *mContext;
	IDXGISwapChain *mSwapChain;
	ID3D11RenderTargetView *mBackBuffer;
	ID3D11DepthStencilView *mDepthBuffer;
	ID3D11SamplerState *mSamplerState;

	DXGI_MODE_DESC mModeDesc;
	DXGI_SAMPLE_DESC mSampleDesc;
	D3D11_VIEWPORT mViewport;

	IDXGIDevice *mpDXGIDevice;

	Size mSize;
	float mClearColor[4];
	
	void CreateDeviceAndSwapChain(const HWND& window);
	void CreateRenderTarget();
};
