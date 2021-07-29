#include "iImage.h"

#include "iStd.h"

iImage::iImage()
{
	arrayTex = new iArray(freeObject);

	ani = false;
	repeatNum = 0;
	repeatIdx = 0;
	frame = 0;
	lastframe = false;
	_delta = 0.08f;
	delta = 0.0f;

	position = iPointZero;
	anc = TOP | LEFT;
	reverse = REVERSE_NONE;

	method = NULL;
	parm = NULL;
}

iImage::~iImage()
{
	delete arrayTex;
}

void iImage::freeObject(void* parm)
{
	freeImage((Texture*)parm);
}

void iImage::addObject(Texture* tex)
{
	arrayTex->addObject(tex);
	tex->retainCount++;
	if (this->tex == NULL)
		this->tex = tex;
}

void iImage::setTexObject(int index)
{
	tex = (Texture*)arrayTex->objectAtIndex(index);
	frame = index;
}

iRect iImage::touchRect(iPoint off, iSize size)
{
	iRect rt;
	rt.origin = position + off - iPointMake(size.width / 2, size.height / 2);
	rt.size = iSizeMake(tex->width, tex->height) + size;

	return rt;
}

void iImage::startAnimation(ANIMATION_METHOD m, void* p)
{
	ani = true;

	repeatIdx = 0;
	frame = 0;

	delta = 0.0f;

	method = m;
	parm = p;
}
void iImage::stopAnimation()
{
	ani = false;
}

void iImage::paint(float dt, iPoint off, iPoint rate)
{
	paint(dt, off.x, off.y, rate.x, rate.y);
}

void iImage::paint(float dt, iPoint off, float rx, float ry)
{
	paint(dt, off.x, off.y, rx, ry);
}
void iImage::paint(float dt, float x, float y, float rx, float ry)
{
	if (ani)
	{
		delta += dt;

		if (delta > _delta) //이번 프레임 끝
		{
			delta = 0.0f;
			
			frame++;
			if (frame > arrayTex->count - 1)
			{
				frame = 0;
				if (repeatNum == 0) //infinite
				{

				}
				else
				{
					repeatIdx++;
					if (repeatIdx == repeatNum)
					{
						if (method)
							method(parm);
						ani = false;
						if (lastframe)
							frame = arrayTex->count - 1;
					}
				}
			}
		}

		
	}

	tex = (Texture*)arrayTex->objectAtIndex(frame);

	drawImage(tex, position.x + x, position.y + y, anc,
		0, 0, tex->width, tex->height, 
		rx, ry, 2, 0, reverse);
}

iImage* iImage::copy()
{
	iImage* img = new iImage();
	iArray* a = img->arrayTex;

	memcpy(img, this, sizeof(iImage));
	img->arrayTex = a;
	for (int i = 0; i < arrayTex->count; i++)
		img->addObject((Texture*)arrayTex->objectAtIndex(i));

	return img;
}