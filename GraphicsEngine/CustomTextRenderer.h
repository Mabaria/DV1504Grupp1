#pragma once
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "Dwrite")

class CustomTextRenderer : public IDWriteTextRenderer
{
public:

	CustomTextRenderer(
		ID2D1Factory* pD2DFactory,
		ID2D1HwndRenderTarget* pRT,
		ID2D1SolidColorBrush* pOutlineBrush,
		ID2D1SolidColorBrush* pFillBrush
	);

	HRESULT STDMETHODCALLTYPE DrawGlyphRun(
		void                         * clientDrawingContext,
		FLOAT                         baselineOriginX,
		FLOAT                         baselineOriginY,
		DWRITE_MEASURING_MODE         measuringMode,
		const DWRITE_GLYPH_RUN             * glyphRun,
		const DWRITE_GLYPH_RUN_DESCRIPTION * glyphRunDescription,
		IUnknown                     * clientDrawingEffect
	) override;


private:
	ID2D1Factory * mpD2DFactory;
	ID2D1HwndRenderTarget *mpRT;
	ID2D1SolidColorBrush *mpOutlineBrush;
	ID2D1SolidColorBrush *mpFillBrush;

};