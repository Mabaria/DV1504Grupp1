#pragma once
#include <d2d1.h>
#include <vector>
#include <Windows.h>
#pragma comment(lib, "d2d1")

class Direct2D
{
public:
	Direct2D();
	~Direct2D();
	void CreateRenderTarget(HWND window, unsigned int width, unsigned int height);

private:

	ID2D1HwndRenderTarget * pRenderTarget;
	ID2D1Factory *pFactory;

	void CreateFactory();
	void Init();
};