#include "Direct2D.h"

Direct2D::Direct2D()
{
	this->pFactory = NULL;
	this->pRenderTarget = NULL;
	this->CreateFactory();
}

Direct2D::~Direct2D()
{
}

void Direct2D::CreateFactory()
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&this->pFactory);
}

void Direct2D::Init()
{

}

void Direct2D::CreateRenderTarget(
	HWND window,
	unsigned int width,
	unsigned int height)
{
	this->pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			window,
			D2D1::SizeU(width, height)),
		&this->pRenderTarget);
}

