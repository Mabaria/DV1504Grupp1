#pragma once
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "Dwrite")

class CustomTextRenderer : public IDWriteTextRenderer
{
public:

	CustomTextRenderer(
		ID2D1Factory* pD2DFactory,
		ID2D1RenderTarget* pRT,
		ID2D1SolidColorBrush* pOutlineBrush,
		ID2D1SolidColorBrush* pFillBrush
	);

	~CustomTextRenderer();

	HRESULT STDMETHODCALLTYPE DrawGlyphRun(
		void                         * clientDrawingContext,
		FLOAT                         baselineOriginX,
		FLOAT                         baselineOriginY,
		DWRITE_MEASURING_MODE         measuringMode,
		const DWRITE_GLYPH_RUN             * glyphRun,
		const DWRITE_GLYPH_RUN_DESCRIPTION * glyphRunDescription,
		IUnknown                     * clientDrawingEffect
	) override;

	HRESULT STDMETHODCALLTYPE DrawUnderline(
		void             * clientDrawingContext,
		FLOAT             baselineOriginX,
		FLOAT             baselineOriginY,
		const DWRITE_UNDERLINE * underline,
		IUnknown         * clientDrawingEffect
	) override;

	HRESULT STDMETHODCALLTYPE DrawInlineObject(
		void                * clientDrawingContext,
		FLOAT                originX,
		FLOAT                originY,
		IDWriteInlineObject * inlineObject,
		BOOL                 isSideways,
		BOOL                 isRightToLeft,
		IUnknown            * clientDrawingEffect
	) override;

	HRESULT STDMETHODCALLTYPE DrawStrikethrough(
		void                 * clientDrawingContext,
		FLOAT                 baselineOriginX,
		FLOAT                 baselineOriginY,
		const DWRITE_STRIKETHROUGH * strikethrough,
		IUnknown             * clientDrawingEffect
	) override;

	HRESULT STDMETHODCALLTYPE GetCurrentTransform(
		void          * clientDrawingContext,
		DWRITE_MATRIX * transform
	) override;

	HRESULT STDMETHODCALLTYPE IsPixelSnappingDisabled(
		void * clientDrawingContext,
		BOOL  * isDisabled
	) override;

	HRESULT STDMETHODCALLTYPE GetPixelsPerDip(
		void  * clientDrawingContext,
		FLOAT * pixelsPerDip
	) override;

	unsigned long STDMETHODCALLTYPE AddRef() override;
	unsigned long STDMETHODCALLTYPE Release() override;
	
	HRESULT STDMETHODCALLTYPE QueryInterface(
		REFIID riid,
		void   **ppvObject
	) override;

	void SetOutlineBrush(ID2D1SolidColorBrush* pOutlineBrush);
	void SetFillBrush(ID2D1SolidColorBrush* pFillBrush);
	void SetRenderTarget(ID2D1RenderTarget* pRenderTarget);

private:
	unsigned long mRefCount;
	ID2D1Factory *mpD2DFactory;
	ID2D1RenderTarget *mpRT;
	ID2D1SolidColorBrush *mpOutlineBrush;
	ID2D1SolidColorBrush *mpFillBrush;

};