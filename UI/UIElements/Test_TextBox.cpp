#include "Test_TextBox.h"

void Test_Text()
{
	Window window(L"Text_Test", 1280, 720);
	Direct2D D2Test(
		window.GetWindow(),
		window.GetClientSize().width,
		window.GetClientSize().height);
	Button butt(D2Test,"",0,0,0,0);
	
	ID2D1HwndRenderTarget* pIRenderTarget;
	
	HRESULT hr = S_OK;
	ID2D1SolidColorBrush* pIRedBrush = NULL;
	IDWriteTextFormat* pITextFormat = NULL;
	IDWriteFactory* pIDWriteFactory = NULL;
	IDWriteTextLayout *pLayout = NULL;
	float lineSpacing;

	

	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pIDWriteFactory));
	}

	if (SUCCEEDED(hr))
	{
		hr = pIDWriteFactory->CreateTextFormat(
			L"Arial",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			13,
			L"en-US",
			&pITextFormat
		);
	}
	//pIDWriteFactory->
	//pITextFormat->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, 0.1, 0.8);
	//pITextFormat->GetLineSpacing(nullptr, &lineSpacing, nullptr);
	//pITextFormat->SetTrimming(DWRITE_TRIMMING_GRANULARITY_WORD)
	//pITextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WHOLE_WORD);
	
	pIDWriteFactory->CreateTextLayout(
		L"Yes I call my dad \"daddy\" unironically, what the fuck of it? It's not gay. I just want him to feel appreciated for once in his life. I bet if I was hitting 50, lived in a sexless marriage and got shit on at work every single day, I would feel a little bit better if my son recognized my alpha status and sexual superiority. Again, it's not fucking gay, I'm just being a good son. In fact, I'd go as far as sucking his dick to satisfy his sexual needs (no homo tho). Fuck you if you think this is weird, you're the one who's fucked up for not appreciating your dad. He should be the one you call daddy, not your scumbag boyfriend who just wants to get his dick wet. He's not the one who raised you and put up with your shit for all these years. RESPECT YOUR MOTHERFUCKIN DADS!",
		wcslen(L"Yes I call my dad \"daddy\" unironically, what the fuck of it? It's not gay. I just want him to feel appreciated for once in his life. I bet if I was hitting 50, lived in a sexless marriage and got shit on at work every single day, I would feel a little bit better if my son recognized my alpha status and sexual superiority. Again, it's not fucking gay, I'm just being a good son. In fact, I'd go as far as sucking his dick to satisfy his sexual needs (no homo tho). Fuck you if you think this is weird, you're the one who's fucked up for not appreciating your dad. He should be the one you call daddy, not your scumbag boyfriend who just wants to get his dick wet. He's not the one who raised you and put up with your shit for all these years. RESPECT YOUR MOTHERFUCKIN DADS!"),
		pITextFormat,
		100,
		100,
		&pLayout
	);
	if (SUCCEEDED(hr))
	{
		hr = D2Test.GetpRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&pIRedBrush
		);
	}
	std::string test = "Yes I call my dad \"daddy\" unironically, what the fuck of it? It's not gay. I just want him to feel appreciated for once in his life. I bet if I was hitting 50, lived in a sexless marriage and got shit on at work every single day, I would feel a little bit better if my son recognized my alpha status and sexual superiority. Again, it's not fucking gay, I'm just being a good son. In fact, I'd go as far as sucking his dick to satisfy his sexual needs (no homo tho). Fuck you if you think this is weird, you're the one who's fucked up for not appreciating your dad. He should be the one you call daddy, not your scumbag boyfriend who just wants to get his dick wet. He's not the one who raised you and put up with your shit for all these years. RESPECT YOUR MOTHERFUCKIN DADS!";
	std::string test2 = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > convert;
	std::wstring result = convert.from_bytes(test);
	const wchar_t *wTest = result.c_str();
	float dpiY=0;
	float dpiX=0;
	D2Test.GetpRenderTarget()->GetDpi(&dpiX, &dpiY);
	float pixelsPerDpiY = dpiY / 96;
	D2D1_RECT_F layoutRect = D2D1::RectF(0.f, 0.f, 105.f, pITextFormat->GetFontSize()*2*pixelsPerDpiY);

	
	window.Open();
	while (window.IsOpen())
	{
		/*layoutRect.bottom += 1;
		layoutRect.left += 1;
		layoutRect.right += 1;
		layoutRect.top += 1;*/

		if (Mouse::GetPositionPercentage().x <
			layoutRect.right / D2Test.GetpRenderTarget()->GetSize().width &&
			Mouse::GetPositionPercentage().x >
			layoutRect.left / D2Test.GetpRenderTarget()->GetSize().width &&
			Mouse::GetPositionPercentage().y <
			layoutRect.bottom / D2Test.GetpRenderTarget()->GetSize().height &&
			Mouse::GetPositionPercentage().y >
			layoutRect.top / D2Test.GetpRenderTarget()->GetSize().height &&
			Mouse::IsButtonDown(Buttons::Left))
		{
			layoutRect.bottom = wcslen(wTest);
		}
		
		window.Update();
		D2Test.GetpRenderTarget()->BeginDraw();
		D2Test.GetpRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
	/*	D2Test.GetpRenderTarget()->DrawTextW(
			wTest,
			wcslen(wTest),
			pITextFormat,
			layoutRect,
			pIRedBrush, 
			D2D1_DRAW_TEXT_OPTIONS_CLIP*/
			
		//);
		D2Test.GetpRenderTarget()->DrawTextLayout(
			D2D1::Point2F(0, 0),
			pLayout,
			pIRedBrush, 
			D2D1_DRAW_TEXT_OPTIONS_CLIP);
		D2Test.GetpRenderTarget()->EndDraw();
	}

	// Actually draw the text at the origin.
	
	
	// Clean up.
	/*SafeRelease(&pIRedBrush);
	SafeRelease(&pITextFormat);
	SafeRelease(&pIDWriteFactory);*/


}
