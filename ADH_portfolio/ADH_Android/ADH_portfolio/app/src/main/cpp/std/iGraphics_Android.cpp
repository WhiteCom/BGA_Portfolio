#include "iGraphics.h"

#if (OS==OS_ANDROID)

#include <jni.h>

#include "iNDK.h"

#include "iStd.h"

// -----------------------------------------------------------------
// class
// -----------------------------------------------------------------
class QC
{
public:
	QC()
	{
        bitmap = NULL;
        canvas = NULL;
        size = iSizeZero;
	}

	virtual ~QC()
	{
		clean();
	}

	void clean()
	{
		if(bitmap == NULL)
			return;

		JNIEnv* env = getEnv();

		ndkFreeBitmap(bitmap);
		env->DeleteGlobalRef(bitmap);
		bitmap = NULL;

		env->DeleteGlobalRef(canvas);
		canvas = NULL;

	}

	void set(jobject bitmap, jobject canvas, iSize size)
	{
		clean();

		this->bitmap = bitmap;
		this->canvas = canvas;
		this->size = size;
	}

	jobject bitmap, canvas;
	iSize size;
};

static QC* qc = NULL;
static jobject paint;

iGraphics::iGraphics()
{
	qc = new QC();

	JNIEnv* env = getEnv();
	paint = ndkLocal2global(ndkNewPaint(env));
    ndkSetStringName(paint, getStringName());
    ndkSetAntialias(paint, true);
    ndkSetTextSize(paint, getStringSize());
    ndkSetTextScaleX(paint, 1.0f);
}

iGraphics* iGraphics::share()
{
	static iGraphics* graphics = NULL;
	if (graphics == NULL)
		graphics = new iGraphics();
	return graphics;
}

iGraphics::~iGraphics()
{
	delete qc;
	JNIEnv* env = getEnv();
	env->DeleteGlobalRef(paint);
}

void iGraphics::init(iSize size)
{
	init(size.width, size.height);
}

void iGraphics::init(int width, int height)
{
	JNIEnv* env = getEnv();

	size_t potWid = nextPot(width);
	size_t potHei = nextPot(height);

	jobject bitmap = ndkLocal2global( ndkCreateBitmap(env, potWid, potHei) );
	jobject canvas = ndkLocal2global( ndkNewCanvas(env, bitmap) );
	qc->set(bitmap, canvas, iSizeMake(width, height));
	clear();
}

void iGraphics::clear(float r, float g, float b, float a)
{
	ndkDrawARGB(qc->canvas, floatToUint8(a), floatToUint8(r), floatToUint8(g), floatToUint8(b));
}
void iGraphics::clear()
{
	ndkDrawARGB(qc->canvas, 0, 0, 0, 0);
}

Texture* iGraphics::getTexture()
{
	if( qc->bitmap==NULL )
		return NULL;
	Texture* tex = ndkCreateImage(qc->bitmap);
	tex->width = qc->size.width;
	tex->height = qc->size.height;

	qc->clean();
	return tex;
}

void iGraphics::drawLine(iPoint sp, iPoint ep)
{
	drawLine(sp.x, sp.y, ep.x, ep.y);
}
void iGraphics::drawLine(float x0, float y0, float x1, float y1)
{
	if( qc->bitmap==NULL )
		return;

	float r, g, b, a;
	getRGBA(r, g, b, a);
	ndkSetARGB(paint, floatToUint8(a), floatToUint8(r), floatToUint8(g), floatToUint8(b));

	ndkSetStrokeWidth(paint, getLineWidth());

	ndkDrawLine(qc->canvas, paint,x0, y0, x1, y1);
}

void iGraphics::drawRect(iRect rt, float radius)
{
	drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}
void iGraphics::drawRect(float x, float y, float width, float height, float radius)
{
	if( qc->bitmap==NULL )
		return;

	ndkSetPaintStyle(paint, "STROKE");

	float r, g, b, a;
	getRGBA(r, g, b, a);
	ndkSetARGB(paint, floatToUint8(a), floatToUint8(r), floatToUint8(g), floatToUint8(b));

	ndkSetStrokeWidth(paint, getLineWidth());

	ndkDrawRect(qc->canvas, paint, x, y, width, height, radius);
}

void iGraphics::fillRect(iRect rt, float radius)
{
	fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}
void iGraphics::fillRect(float x, float y, float width, float height, float radius)
{
	if( qc->bitmap==NULL )
		return;

	ndkSetPaintStyle(paint, "FILL");

	float r, g, b, a;
	getRGBA(r, g, b, a);
	ndkSetARGB(paint, floatToUint8(a), floatToUint8(r), floatToUint8(g), floatToUint8(b));

	//ndkSetStrokeWidth(qc->paint, getLineWidth());

	ndkDrawRect(qc->canvas, paint, x, y, width, height, radius);
}

igImage* iGraphics::createIgImage(const char* szFormat, ...)
{
	char szText[1024];
	va_start_end(szText, szFormat);
	int length = 0;
	char* buf = getStream(szText, length);
	jobject bitmap = ndkCreateBitmap(buf, length);
	if( bitmap==NULL )
	{
		return NULL;
	}

	return ndkLocal2global( bitmap );
}

void iGraphics::freeIgImage(igImage* img)
{
	jobject bitmap = (jobject)img;
	if( bitmap )
	{
		ndkFreeBitmap(bitmap);

		JNIEnv* env = getEnv();
		env->DeleteGlobalRef(bitmap);
	}
}

int iGraphics::igImageGetWidth(igImage* img)
{
	return ndkGetBitmapSize((jobject)img).width;
}
int iGraphics::igImageGetHeight(igImage* img)
{
	return ndkGetBitmapSize((jobject)img).height;
}

void iGraphics::drawigImage(igImage* img, float x, float y, int anc,
							float ix, float iy, float iw, float ih,
							float rx, float ry,
							int xyz, float degree)
{
	float r, g, b, a;
	getRGBA(r, g, b, a);
	ndkSetARGB(paint, floatToUint8(a), floatToUint8(r), floatToUint8(g), floatToUint8(b));

	jobject bitmap = (jobject)img;
	iSize size = ndkGetBitmapSize(bitmap);
	size.width *= rx;
	size.height *= ry;

	if(iw == (float)igImageGetWidth(img))
		ndkDrawBitmap(qc->canvas, paint, bitmap, x, y, size.width, size.height, anc);
	else
		ndkDrawBitmap(	qc->canvas, paint, bitmap, x, y, size.width, size.height, anc,
			   			ix, iy, iw, ih);
}

void iGraphics::drawigImage(igImage* img, float x, float y, int anc)
{
	drawigImage(img, x, y, TOP | LEFT,
				0, 0, igImageGetWidth(img), igImageGetHeight(img),
				1.0f, 1.0f,
				2, 0);
}

iRect iGraphics::rectOfString(const char* szFormat, ...)
{
    char szText[512];
    va_start_end(szText, szFormat);

    ndkSetStringName(paint, getStringName());
    ndkSetTextSize(paint, getStringSize());
    iRect rect = ndkRectOfString(paint, szText);
    float b = getStringBorder();
    if(b)
    {
        rect.origin.x -= b / 2;
        rect.origin.y -= b / 2;
        rect.size.width += b;
        rect.size.height += b;
    }
	return rect;
}

void iGraphics::drawString(float x, float y, int anc, const char* szFormat, ...)
{
	if( qc->bitmap==NULL )
		return;

	char sz[1024];
	va_start_end(sz, szFormat);

	char szText[1024];
	sprintf(szText, sz, 0);

	iRect r = rectOfString(szText);
	x -= r.origin.x;
	y -= r.origin.y;

	switch (anc) {
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

	{
		float r, g, b, a;
		float stringBorder = getStringBorder();
		if( stringBorder )
		{
			ndkSetPaintStyle(paint, "STROKE");
			ndkSetStrokeWidth(paint, stringBorder);
			getStringBorderRGBA(r, g, b, a);
			ndkSetARGB(paint, floatToUint8(a), floatToUint8(r), floatToUint8(g), floatToUint8(b));
			ndkDrawChar(qc->canvas, szText, x, y, paint);
		}
		ndkSetPaintStyle(paint, "FILL");
		getStringRGBA(r, g, b, a);
		ndkSetARGB(paint, floatToUint8(a), floatToUint8(r), floatToUint8(g), floatToUint8(b));
		ndkDrawChar(qc->canvas, szText, x, y, paint);
	}
}

#endif// (OS==OS_ANDROID)