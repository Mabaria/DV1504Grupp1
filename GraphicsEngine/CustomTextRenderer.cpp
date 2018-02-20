#include "CustomTextRenderer.h"

CustomTextRenderer::CustomTextRenderer(
	ID2D1Factory* pD2DFactory,
	ID2D1HwndRenderTarget* pRT,
	ID2D1SolidColorBrush* pOutlineBrush,
	ID2D1SolidColorBrush* pFillBrush
)
	:
	mpD2DFactory(pD2DFactory),
	mpRT(pRT),
	mpOutlineBrush(pOutlineBrush),
	mpFillBrush(pFillBrush)
{
	mpD2DFactory->AddRef();
	mpRT->AddRef();
	mpOutlineBrush->AddRef();
	mpFillBrush->AddRef();

}

CustomTextRenderer::~CustomTextRenderer()
{
	if (this->mpD2DFactory)
	{
		this->mpD2DFactory->Release();
	}
	if (this->mpRT)
	{
		this->mpRT->Release();
	}
	if (this->mpOutlineBrush)
	{
		this->mpOutlineBrush->Release();
	}
	if (this->mpFillBrush)
	{
		this->mpFillBrush->Release();
	}
}

HRESULT CustomTextRenderer::DrawGlyphRun(
	void * clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	DWRITE_MEASURING_MODE measuringMode,
	const DWRITE_GLYPH_RUN * glyphRun,
	const DWRITE_GLYPH_RUN_DESCRIPTION * glyphRunDescription,
	IUnknown * clientDrawingEffect)
{
	HRESULT hr;
	// Create path geometry
	ID2D1PathGeometry *pPathGeometry = nullptr;
	hr = this->mpD2DFactory->CreatePathGeometry(
		&pPathGeometry
	);

	// Open the path geometry using the geometry sink
	ID2D1GeometrySink *pSink = nullptr;
	if (SUCCEEDED(hr))
	{
		hr = pPathGeometry->Open(
			&pSink
		);
	}

	// Get the outline geometries from DirectWrite and place in geometry sink
	if (SUCCEEDED(hr))
	{
		hr = glyphRun->fontFace->GetGlyphRunOutline(
			glyphRun->fontEmSize,
			glyphRun->glyphIndices,
			glyphRun->glyphAdvances,
			glyphRun->glyphOffsets,
			glyphRun->glyphCount,
			glyphRun->isSideways,
			glyphRun->bidiLevel % 2,
			pSink
		);
	}
	// Close the geometry sink
	if (SUCCEEDED(hr))
	{
		hr = pSink->Close();
	}
	// Create matrix for glyph run origin translation
	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		baselineOriginX, baselineOriginY
	);

	// Create the transformed geometry
	ID2D1TransformedGeometry *pTransformedGeometry = NULL;
	if (SUCCEEDED(hr))
	{
		hr = this->mpD2DFactory->CreateTransformedGeometry(
			pPathGeometry,
			&matrix,
			&pTransformedGeometry
		);
	}

	//! ---------------------------------------------------------
	//! Actual drawing happens down here
	//! ---------------------------------------------------------

	this->mpRT->DrawGeometry(
		pTransformedGeometry,
		this->mpOutlineBrush
	);
	this->mpRT->FillGeometry(
		pTransformedGeometry,
		this->mpFillBrush
	);

	// Release created objects
	pPathGeometry->Release();
	pSink->Release();
	pTransformedGeometry->Release();

	return hr;
}

HRESULT CustomTextRenderer::DrawUnderline(void * clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, const DWRITE_UNDERLINE * underline, IUnknown * clientDrawingEffect)
{
	return E_NOTIMPL;
}

HRESULT CustomTextRenderer::DrawInlineObject(void * clientDrawingContext, FLOAT originX, FLOAT originY, IDWriteInlineObject * inlineObject, BOOL isSideways, BOOL isRightToLeft, IUnknown * clientDrawingEffect)
{
	return E_NOTIMPL;
}
