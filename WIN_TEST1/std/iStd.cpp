#include "iStd.h"

#include "iWindow.h"

iSize devSize;
iRect viewport;

bool* keyBuf;

void appInitialize()
{
	srand(time(NULL));
	keyBuf = new bool[256];
	memset(keyBuf, 0xff, 256);
}

int keyStat = 0, keyDown = 0; //stat : 계속 누름, down : 한번누름
void setKeys(int& keys, iKeyStat stat, unsigned int c);

void setKeyStat(iKeyStat stat, unsigned int c)
{
	setKeys(keyStat, stat, c);
}

void setKeyDown(iKeyStat stat, unsigned int c)
{
	//just one time
	if (stat == iKeyStatBegan)
	{
		if (keyBuf[c])
			return;
		keyBuf[c] = true;
		setKeys(keyDown, stat, c);
	}
	else
	{
		keyBuf[c] = false;
	}
}

void setKeys(int& keys, iKeyStat stat, unsigned int c)
{
	if (stat == iKeyStatBegan)
	{
		if (c == 'a' || c == 'A')
			keys |= keysA;
		else if (c == 's' || c == 'S')
			keys |= keysS;
		else if (c == 'd' || c == 'D')
			keys |= keysD;
		else if (c == 'w' || c == 'W')
			keys |= keysW;
		else if (c == ' ')
			keys |= keysSpace;

		else if (c == VK_LEFT)
			keys |= keysLeft;
		else if (c == VK_RIGHT)
			keys |= keysRight;
		else if (c == VK_UP)
			keys |= keysUp;
		else if (c == VK_DOWN)
			keys |= keysDown;
	}
	
	else if (stat == iKeyStatEnded)
	{
		if (c == 'a' || c == 'A')
			keys &= ~keysA;
		else if (c == 's' || c == 'S')
			keys &= ~keysS;
		else if (c == 'd' || c == 'D')
			keys &= ~keysD;
		else if (c == 'w' || c == 'W')
			keys &= ~keysW;
		else if (c == ' ')
			keys &= ~keysSpace;

		else if (c == VK_LEFT)
			keys &= ~keysLeft;
		else if (c == VK_RIGHT)
			keys &= ~keysRight;
		else if (c == VK_UP)
			keys &= ~keysUp;
		else if (c == VK_DOWN)
			keys &= ~keysDown;
	}
}

int random()
{
	return rand();
}

float _cos(float degree)
{
	return cos(degree * M_PI / 180);
}
float _sin(float degree)
{
	return sin(degree * M_PI / 180);
}

float uint8ToFloat(uint8 c)
{
	return c / 255.0f;
}

uint8 floatToUint8(float c)
{
	return (uint8)(c * 0xFF);
}

static float _r = 1.0f, _g = 1.0f, _b = 1.0f, _a = 1.0f;

void setRGBA(iColor4f c)
{
	_r = c.r;
	_g = c.g;
	_b = c.b;
	_a = c.a;
}

void setRGBA(iColor4b c)
{
	_r = uint8ToFloat(c.r);
	_g = uint8ToFloat(c.g);
	_b = uint8ToFloat(c.b);
	_a = uint8ToFloat(c.a);
}
void setRGBA(float r, float g, float b, float a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void getRGBA(float& r, float& g, float& b, float& a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}


void clearRect()
{
	graphics->Clear(Color(
		floatToUint8(_a),
		floatToUint8(_r),
		floatToUint8(_g),
		floatToUint8(_b)
	));
}

void setClip(float x, float y, float width, float height)
{
	if (width == 0.0f || height == 0.0f)
		graphics->SetClip(Rect(0, 0, devSize.width, devSize.height));
	else
		graphics->SetClip(Rect(x, y, width, height));

}

void drawRect(iRect rt, float radius)
{
	drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}
void drawRect(float x, float y, float width, float height, float radius)
{
	Pen pen(Color(
		floatToUint8(_a),
		floatToUint8(_r),
		floatToUint8(_g),
		floatToUint8(_b)
	));

	pen.SetWidth(1.0f);

	GraphicsPath path;
	path.AddLine(x + radius, y, x + width - (radius * 2), y);
	path.AddArc(x + width - (radius * 2), y, radius * 2, radius * 2, 270, 90);
	path.AddLine(x + width, y + radius, x + width, y + height - (radius * 2));
	path.AddArc(x + width - (radius * 2), y + height - (radius * 2), radius * 2, radius * 2, 0, 90);
	path.AddLine(x + width - (radius * 2), y + height, x + radius, y + height);
	path.AddArc(x, y + height - (radius * 2), radius * 2, radius * 2, 90, 90);
	path.AddLine(x, y + height - (radius * 2), x, y + radius);
	path.AddArc(x, y, radius * 2, radius * 2, 180, 90);
	path.CloseFigure();

	graphics->DrawPath(&pen, &path);
}

void fillRect(iRect rt, float radius)
{
	fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}
void fillRect(float x, float y, float width, float height, float radius)
{
	SolidBrush brush(Color(
		floatToUint8(_a),
		floatToUint8(_r),
		floatToUint8(_g),
		floatToUint8(_b)
	));

	GraphicsPath path;
	path.AddLine(x + radius, y, x + width - (radius * 2), y);
	path.AddArc(x + width - (radius * 2), y, radius * 2, radius * 2, 270, 90);
	path.AddLine(x + width, y + radius, x + width, y + height - (radius * 2));
	path.AddArc(x + width - (radius * 2), y + height - (radius * 2), radius * 2, radius * 2, 0, 90);
	path.AddLine(x + width - (radius * 2), y + height, x + radius, y + height);
	path.AddArc(x, y + height - (radius * 2), radius * 2, radius * 2, 90, 90);
	path.AddLine(x, y + height - (radius * 2), x, y + radius);
	path.AddArc(x, y, radius * 2, radius * 2, 180, 90);
	path.CloseFigure();

	graphics->FillPath(&brush, &path);
}

int igImageWidth(igImage* ig)
{
	return ((Image*)ig)->GetWidth();
}
int igImageHeight(igImage* ig)
{
	return ((Image*)ig)->GetHeight();
}

Texture** createImageDivide(int numX, int numY, const char* szFormat, ...)
{
	char szText[1024];
	va_start_end(szText, szFormat);

	wchar_t* ws = utf8_to_utf16(szText);
	Bitmap* bmp = Bitmap::FromFile(ws, false);
	delete ws;

	// lock
	int width = bmp->GetWidth();
	int height = bmp->GetHeight();
	Rect rect(0, 0, width, height);
	BitmapData bmpData;
	bmp->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

	int stride = bmpData.Stride / 4;
	uint32* rgba = (uint32*)bmpData.Scan0;

	// to do...
	int numXY = numX * numY;
	Texture** texs = new Texture * [numXY];
	int w = width / numX;
	int h = height / numY;
	for (int i = 0; i < numXY; i++)
	{
		int x = i % numX;
		int y = i / numX;

		Bitmap* b = new Bitmap(w, h, PixelFormat32bppARGB);
		Rect rt(0, 0, w, h);
		BitmapData bd;
		b->LockBits(&rt, ImageLockModeWrite, PixelFormat32bppARGB, &bd);

		int srd = bd.Stride / 4;
		uint32* color = (uint32*)bd.Scan0;
		for (int j = 0; j < h; j++)
		{
			memcpy(&color[srd * j],
				&rgba[stride * (h * y + j) + w * x],
				sizeof(uint32) * w);
		}

		b->UnlockBits(&bd);

		Texture* tex = new Texture;
		tex->texID = b;
		tex->width = w;
		tex->height = h;
		tex->potWidth = w;
		tex->potHeight = h;
		tex->retainCount = 1;

		texs[i] = tex;
	}

	// unlock
	bmp->UnlockBits(&bmpData);

	return texs;
}

Texture* createImage(const char* szFormat, ...)
{
	char szText[1024];
	va_start_end(szText, szFormat);

	wchar_t* ws = utf8_to_utf16(szText);
	Image* img = Image::FromFile(ws, false);
	delete ws;

	Texture* tex = new Texture;
	tex->texID = img;
	tex->width = img->GetWidth();
	tex->height = img->GetHeight();
	tex->potWidth = tex->width;
	tex->potHeight = tex->height;
	return tex;
}

void freeImage(Texture* tex)
{
	if (tex->retainCount > 1)
	{
		tex->retainCount--;
		return;
	}
	delete (Image*)tex->texID;
	delete tex;
}

void drawImage(Texture* tex, float x, float y, int anc,
	float ix, float iy, float iw, float ih,
	float rx, float ry,
	int xyz, float degree, int reverse)
{
	//Image* img = (Image*)_img;
	float w = iw * rx, h = ih * ry;

	switch (anc)
	{
	case TOP | LEFT:										break;
	case TOP | RIGHT:			x -= w;						break;
	case TOP | HCENTER:			x -= w / 2;					break;

	case BOTTOM | LEFT:						y -= h;			break;
	case BOTTOM | RIGHT:		x -= w;		y -= h;			break;
	case BOTTOM | HCENTER:		x -= w / 2; y -= h;			break;

	case VCENTER | LEFT:					y -= h / 2;		break;
	case VCENTER | RIGHT:		x -= w;		y -= h / 2;		break;
	case VCENTER | HCENTER:		x -= w / 2;	y -= h / 2;		break;
	}

	iPoint dstPoint[3] = {
		{x, y}, {x + w, y}, { x, y + h }
	};
	iPoint t = iPointMake(x + w / 2, y + h / 2);
	if (xyz == 0)
	{
		dstPoint[0].y =
		dstPoint[1].y = y + h / 2 - h / 2 * _cos(degree);
		dstPoint[2].y = y + h / 2 + h / 2 * _sin(degree);
	}
	else if (xyz == 1)
	{
		dstPoint[0].x =
		dstPoint[2].x = x + w / 2 - w / 2 * _cos(degree);
		dstPoint[1].x = x + w / 2 + w / 2 * _sin(degree);
	}

	else //if(xyz == 2)
	{
		for (int i = 0; i < 3; i++)
			dstPoint[i] = iPointRotate(dstPoint[i], t, degree);
	}

	if (reverse == REVERSE_NONE);
	else if (reverse == REVERSE_WIDTH) //좌우 반전
	{
		//dstPoint[0]	dstPoint[1]
		//dstPoint[2]	dstPoint[3]

		float t = dstPoint[0].x;
		dstPoint[0].x = dstPoint[1].x;
		dstPoint[1].x = t;

		dstPoint[2].x += w;
	}

	else if (reverse == REVERSE_HEIGHT) //상하 반전
	{
		//dstPoint[0]	dstPoint[1]
		//dstPoint[2]	dstPoint[3]
		float t = dstPoint[0].y;
		dstPoint[0].y = dstPoint[2].y;
		dstPoint[2].y = t;

		dstPoint[1].y += h;	
	}


	ColorMatrix matrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,   _a, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes attr;
	attr.SetColorMatrix(&matrix,
		ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	graphics->DrawImage((Image*)tex->texID, (PointF*)dstPoint, 3,
		ix, iy, iw, ih, UnitPixel, &attr);

}
void drawImage(Texture* tex, float x, float y, int anc)
{
	//Image* img = (Image*)_img;

	drawImage(tex, x, y, anc,
		0, 0, tex->width, tex->height,
		1.0f, 1.0f,
		2, 0);
}

static char* stringName = NULL;
static float stringSize = 20.f;
static float stringR = 1.0f, stringG = 1.0f, stringB = 1.0f, stringA = 1.0f;
static float stringBorder = 0.0f;
static float stringBorderR = 1.0f, stringBorderG = 1.0f, stringBorderB = 1.0f, stringBorderA = 1.0f;

const char* getStringName()
{
	return stringName;
}
void setStringName(const char* str)
{
	if (stringName)
	{
		if (strcmp(stringName, str) == 0)
			return;

		delete stringName;
	}

	stringName = new char[strlen(str) + 1];
	strcpy(stringName, str);
}

float getStringSize()
{
	return stringSize;
}
void setStringSize(float size)
{
	stringSize = size;
}

void getStringRGBA(float& r, float& g, float& b, float& a)
{
	stringR = r;
	stringG = g;
	stringB = b;
	stringA = a;
}

void setStringRGBA(float r, float g, float b, float a)
{
	stringBorderR = r;
	stringBorderG = g;
	stringBorderB = b;
	stringBorderA = a;
}

float getStringBorder()
{
	return stringBorder;
}
void setStringBorder(float border)
{
	stringBorder = border;
}

void getStringBorderRGBA(float& r, float& g, float& b, float& a)
{
	r = stringBorderR;
	g = stringBorderG;
	b = stringBorderB;
	a = stringBorderA;
}

void setStringBorderRGBA(float r, float g, float b, float a)
{
	stringBorderR = r;
	stringBorderG = g;
	stringBorderB = b;
	stringBorderA = a;
}

void checkFontFamily(FontFamily* ff, int& fontStyle)
{
	const char* path = getStringName();
	if (path[0] != 'a' || path[1] != 's' || path[2] != 's') //assets
	{
		wchar_t* wstr = utf8_to_utf16(path);
		FontFamily f(wstr);
		delete wstr;
		fontStyle = FontStyleRegular;

		memcpy(ff, &f, sizeof(FontFamily));
	}
	else
	{
		PrivateFontCollection* pfc = new PrivateFontCollection();

		wchar_t* wstr = utf8_to_utf16(getStringName());
		pfc->AddFontFile(wstr);
		delete wstr;

		int count = pfc->GetFamilyCount();
		int found;
		pfc->GetFamilies(count, ff, &found);

		delete pfc;

		for (int i = 0; i < found; i++)
		{
			//ff->IsAvilable();
			if (ff->IsStyleAvailable(FontStyleRegular))
			{
				fontStyle = FontStyleRegular;
				return;
			}
			else if (ff->IsStyleAvailable(FontStyleBold))
			{
				fontStyle = FontStyleBold;
				return;
			}
		}
	}
}

void drawString(float x, float y, int anc, const char* szFormat, ...)
{
	char szText[1024];
	va_start_end(szText, szFormat);

	iRect r = rectOfString(szText);
	x -= r.origin.x;
	y -= r.origin.y;

	switch (anc)
	{
	case TOP | LEFT:														break;
	case TOP | RIGHT:		x -= r.size.width;								break;
	case TOP | HCENTER:		x -= r.size.width / 2;							break;

	case BOTTOM | LEFT:								y -= r.size.height;		break;
	case BOTTOM | RIGHT:	x -= r.size.width;		y -= r.size.height;		break;
	case BOTTOM | HCENTER:	x -= r.size.width / 2;	y -= r.size.height;		break;

	case VCENTER | LEFT:							y -= r.size.height / 2;	break;
	case VCENTER | RIGHT:	x -= r.size.width;		y -= r.size.height / 2;	break;
	case VCENTER | HCENTER:	x -= r.size.width / 2;	y -= r.size.height / 2;	break;
	}

	PointF position(x, y);

	FontFamily ff;
	int fontStyle;
	checkFontFamily(&ff, fontStyle);
	StringFormat sf;

	GraphicsPath path;
	wchar_t* wstr = utf8_to_utf16(szText);
	path.AddString(wstr, wcslen(wstr), &ff, fontStyle, getStringSize(), position, &sf);
	delete wstr;

	float fr, fg, fb, fa;

	if (getStringBorder())
	{
		getStringBorderRGBA(fr, fg, fb, fa);

		Pen pen(Color(floatToUint8(fa),
			floatToUint8(fr),
			floatToUint8(fg),
			floatToUint8(fb)), stringBorder);

		pen.SetLineJoin(LineJoinRound);
		graphics->DrawPath(&pen, &path);
	}
	getStringRGBA(fr, fg, fb, fa);
	SolidBrush brush(Color(floatToUint8(fa),
		floatToUint8(fr),
		floatToUint8(fg),
		floatToUint8(fb)));

	graphics->FillPath(&brush, &path);
}

class StringSize
{
public:
	Bitmap* bmp;
	Graphics* g;
public:
	StringSize()
	{
		bmp = new Bitmap(devSize.width, devSize.height, PixelFormat32bppARGB);
		g = Graphics::FromImage(bmp);
	}
	virtual ~StringSize()
	{
		delete bmp;
		delete g;
	}

	iRect rect(const char* szText)
	{
		g->Clear(Color(0, 0, 0, 0));

		PointF position(0, 0);

		FontFamily ff;
		int fontStyle;
		checkFontFamily(&ff, fontStyle);
		StringFormat sf;

		GraphicsPath path;
		wchar_t* wstr = utf8_to_utf16(szText);
		path.AddString(wstr, wcslen(wstr), &ff, fontStyle, getStringSize(), position, &sf);
		delete wstr;

		float fr, fg, fb, fa;
		if (getStringBorder())
		{
			getStringBorderRGBA(fr, fg, fb, fa);

			Pen pen(Color(floatToUint8(fa),
				floatToUint8(fr),
				floatToUint8(fg),
				floatToUint8(fb)), stringBorder);

			pen.SetLineJoin(LineJoinRound);

		}
		getStringRGBA(fr, fg, fb, fa);
		SolidBrush brush(Color(floatToUint8(fa),
			floatToUint8(fr),
			floatToUint8(fg),
			floatToUint8(fb)));

		g->FillPath(&brush, &path);

		Rect rect(0, 0, devSize.width, devSize.height);
		BitmapData bmpData;
		bmp->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

		bmpData.Stride;
		//uint32* pixels = (uint32*)bmpData.Scan0;
		uint8* rgba = (uint8*)bmpData.Scan0;
		// 0 ====> width - 1
		int left = bmpData.Width;
		for (int i = 0; i < bmpData.Width; i++)
		{
			bool exist = false;
			for (int j = 0; j < bmpData.Height; j++)
			{
				if (rgba[bmpData.Stride * j + 4 * i + 3])
				{
					exist = true;
					break;
				}
			}
			if (exist)
			{
				left = i;
				break;
			}
		}
		// 0 <===== width - 1 
		int right = -1;
		for (int i = bmpData.Width - 1; i > -1; i--)
		{
			bool exist = false;
			for (int j = 0; j < bmpData.Height; j++)
			{
				if (rgba[bmpData.Stride * j + 4 * i + 3])
				{
					exist = true;
					break;
				}
			}
			if (exist)
			{
				right = i;
				break;
			}
		}
		if (right < left)
		{
			bmp->UnlockBits(&bmpData);
			return iRectMake(0, 0, 0, 0);
		}

		int top;
		for (int j = 0; j < bmpData.Height; j++)
		{
			bool exist = false;
			for (int i = 0; i < bmpData.Width; i++)
			{
				if (rgba[bmpData.Stride * j + 4 * i + 3])
				{
					exist = true;
					break;
				}
			}

			if (exist)
			{
				top = j;
				break;
			}
		}

		int bottom;
		for (int j = bmpData.Height - 1; j > -1; j--)
		{
			bool exist = false;
			for (int i = 0; i < bmpData.Width; i++)
			{
				if (rgba[bmpData.Stride * j + 4 * i + 3])
				{
					exist = true;
					break;
				}
			}

			if (exist)
			{
				bottom = j;
				break;
			}
		}

		bmp->UnlockBits(&bmpData);
		if (bottom < top)
		{
			return iRectMake(0, 0, 0, 0);
		}

		return iRectMake(left, top, right - left, bottom - top);
	}
};

static StringSize* ss = NULL;

iRect rectOfString(const char* szFormat, ...)
{
	if (ss == NULL)
		ss = new StringSize();

	char szText[1024];
	va_start_end(szText, szFormat);

	return ss->rect(szText);
}
