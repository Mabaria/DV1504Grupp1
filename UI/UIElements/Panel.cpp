#include "Panel.h"

Panel::Panel(int width, int height, int top, int left, HWND handle, LPCTSTR title)
{
	this->mWidth = width;
	this->mHeight = height;
	this->mTop = top;
	this->mLeft = left;
	this->mParentWindow = handle;
	this->mTitle = title;

	this->mDirect2D = new Direct2D();

	// Creating a child window that will be the canvas to draw on for the panel.
	this->mPanelWindow = CreateWindowEx(
		0,
		this->mTitle,
		this->mTitle,
		WS_CHILD | WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		this->mLeft,
		this->mTop,
		this->mWidth,
		this->mHeight,
		this->mParentWindow,
		0,
		GetModuleHandle(0),
		0);
	ShowWindow(this->mPanelWindow, SW_NORMAL);

	RECT parent_size;
	GetClientRect(this->mParentWindow, &parent_size);

	this->mParentHeight = parent_size.bottom - parent_size.top;
	this->mParentWidth	= parent_size.right - parent_size.left;
	this->mParentTop	= parent_size.top;
	this->mParentLeft	= parent_size.left;
	this->mIsVisible	= true;
}

Panel::~Panel()
{
	for (unsigned int i = 0; i < this->mBitmapVector.size(); i++)
	{
		if (this->mBitmapVector[i].bitmap)
		{
			this->mBitmapVector[i].bitmap->Release();
			this->mBitmapVector[i].bitmap = nullptr;
		}
	}
	this->mBitmapVector.clear();
	delete this->mDirect2D;
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

const void Panel::ScaleX(int scale)
{
	this->mLeft = scale * this->mLeft;
}

const void Panel::ScaleY(int scale)
{
	this->mTop = scale * this->mTop;
}

const bool Panel::Intersects(int x, int y)
{
	// Assumes intersection.
	bool result = true;

	// If it doesn't intersect in either direction it doesn't intersect.
	// Two statements for readability.
	if (x < this->mLeft || x >(this->mLeft + this->mWidth))
	{
		result = false;
	}
	else if (y < this->mTop || y >(this->mTop + this->mHeight))
	{
		result = false;
	}
	return result;
}

const Fraction Panel::IntersectionFraction(int x, int y)
{
	// Returns the fraction as -1, -1 in case 
	// the click does not intersect the panel.
	Fraction fraction;
	fraction.x = -1.0f;
	fraction.y = -1.0f;

	if (Intersects(x, y))
	{
		fraction.x = ((float)x - (float)this->mLeft) / (float)this->mWidth;
		fraction.y = ((float)y - (float)this->mTop) / (float)this->mHeight;
	}
	return fraction;
}

const bool Panel::UpdateWindowSize()
{
	bool updated = false;

	// Getting the client (parent) size.
	RECT client_size;
	GetClientRect(this->mParentWindow, &client_size);
	int client_height = client_size.bottom - client_size.top;
	int client_width = client_size.right - client_size.left;

	// Change the panel size if the parent has changed.
	if ((this->mParentWidth != client_width ||
		this->mParentHeight != client_height)
		&& client_height != 0
		&& client_width != 0)
	{
		updated = true;
		// Getting how much the client changed in width and height.
		float height_difference_factor =
			(float)client_height / (float)this->mParentHeight;
		float width_difference_factor =
			(float)client_width / (float)this->mParentWidth;

		// Applying these fractions to the member sizes and positions.
		this->mHeight = (int)round(this->mHeight * height_difference_factor);
		this->mTop = (int)round(this->mTop	* height_difference_factor);
		this->mWidth = (int)round(this->mWidth * width_difference_factor);
		this->mLeft = (int)round(this->mLeft * width_difference_factor);

		// Updating the parent size members.
		this->mParentHeight = client_height;
		this->mParentWidth = client_width;
		this->mParentTop = client_size.top;
		this->mParentLeft = client_size.left;

		// Setting the new position and size to the panel window.
		SetWindowPos(
			this->mPanelWindow,
			HWND_TOP, // Z order, ignored by SWP_NOZORDER. 
			this->mLeft,
			this->mTop,
			this->mWidth,
			this->mHeight,
			SWP_NOZORDER);
	}
	return updated;
}

void Panel::UpdateWindowPos()
{
	MoveWindow(this->mPanelWindow,
		this->mLeft,
		this->mTop,
		this->mWidth,
		this->mHeight,
		false);
}

bool Panel::IsMouseInsidePanel()
{
	bool result = false;
	if (this->mIsVisible)
	{
		RECT window_rect;
		GetWindowRect(this->mPanelWindow, &window_rect);
		POINT mouse_pos;
		GetCursorPos(&mouse_pos);
		result = PtInRect(&window_rect, mouse_pos); // if mouse is inside panel
	}
	return result;
}

bool Panel::IsVisible()
{
	return this->mIsVisible;
}

void Panel::Hide()
{
	this->mIsVisible = false;
	MoveWindow(this->mPanelWindow, 0, 0, 0, 0, TRUE);
}

void Panel::Show()
{
	this->mIsVisible = true;
	this->UpdateWindowPos();
	ShowWindow(this->mPanelWindow, SW_SHOW);
}

void Panel::ShowOnTop()
{
	this->Show();
	
	SetWindowPos(this->mPanelWindow, 
		HWND_TOP, 0, 0, 0, 0, 
		SWP_SHOWWINDOW | 
		SWP_NOSIZE | 
		SWP_NOMOVE);
}

HWND *Panel::GetPanelWindowHandle()
{
	return &this->mPanelWindow;
}

void Panel::LoadImageToBitmap(std::string imageFilePath, std::string bitmapName)
{
	BitmapInfo new_bitmap_struct;
	new_bitmap_struct.name = bitmapName;
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> convert;
	std::wstring w_file_path = convert.from_bytes(imageFilePath);

	IWICFormatConverter *converter = this->mDirect2D->GetpFormatConverter();
	IWICBitmapDecoder *decoder = this->mDirect2D->GetpBitmapDecoder();
	IWICBitmapFrameDecode *bitmapSrc = this->mDirect2D->GetpBitmapSrc();

	this->mDirect2D->GetpImagingFactory()->CreateFormatConverter(&converter);
	this->mDirect2D->SetpFormatConverter(converter);
	this->mDirect2D->GetpImagingFactory()->CreateDecoderFromFilename(
		w_file_path.c_str(),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&decoder);
	this->mDirect2D->SetpBitmapDecoder(decoder);
	if (this->mDirect2D->GetpBitmapDecoder() != nullptr)
	{
		this->mDirect2D->GetpBitmapDecoder()->GetFrame(0, &bitmapSrc);
		this->mDirect2D->SetpBitmapSrc(bitmapSrc);
		this->mDirect2D->GetpFormatConverter()->Initialize(
			bitmapSrc,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut);
		if (this->mDirect2D->GetpRenderTarget())
		{
			this->mDirect2D->GetpRenderTarget()->CreateBitmapFromWicBitmap(
				this->mDirect2D->GetpFormatConverter(),
				NULL,
				&new_bitmap_struct.bitmap);
		}
		else
		{
			this->mDirect2D->GetpContext()->CreateBitmapFromWicBitmap(
				this->mDirect2D->GetpFormatConverter(),
				&new_bitmap_struct.bitmap);
		}
	}

	this->mBitmapVector.push_back(new_bitmap_struct);
}

ID2D1Bitmap * Panel::GetBitmapByName(std::string bitmapName)
{
	ID2D1Bitmap *to_return = nullptr; // Default return is nullptr
	std::vector<BitmapInfo>::iterator it;

	for (it = this->mBitmapVector.begin();
		it != this->mBitmapVector.end();
		++it)
	{
		if (bitmapName.compare((*it).name) == 0) // Button with correct name found
		{
			to_return = (*it).bitmap; // Return pointer to button
			it = this->mBitmapVector.end() - 1; // Set iterator to end
												// -1 because incrementation is performed after this.
												// Incrementing on .end() is a baaad idea.
		}
	}
	return to_return;
}


