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

struct Zoom
{
	bool ing;
	float delta, _delta;
	float delay, _delay;
	float scale;
	iPoint position;
};

static Zoom zoom =
{
	false,
	0.5f, 0.5f,
	0.5f, 0.5f,
	1.0f,
	{0,0},
};

bool setZoom(iPoint point, float scale, float deltaZoom, float deltaDelay)
{
	if (zoom.ing)
		return false;

	zoom.ing = true;
	zoom.position = point;
	zoom.scale = scale;
	zoom._delta = deltaZoom;
	zoom.delta = 0.0f;
	zoom._delay = deltaDelay;
	zoom.delay = 0.0f;

	return true;
}

float updateZoom(float dt, iPoint& p)
{
	float s = 1.0f;
	if (zoom.ing == false)
		return s;

	if (zoom.delta < zoom._delta)
	{
		s = linear(zoom.delta / zoom.delta, 1.0f, zoom.scale);

		zoom.delta += dt;
		if (zoom.delta > zoom._delta)
			zoom.delta = zoom._delta;

	}
	else if (zoom.delta == zoom._delta)
	{
		s = zoom.scale;

		zoom.delay += dt;
		if (zoom.delay > zoom._delay)
			zoom.delta += 0.000001f;
	}
	else if (zoom.delta < zoom._delta * 2)
	{
		s = linear((zoom.delta - zoom._delta) / zoom._delta, zoom.scale, 1.0f);

		zoom.delta += dt;
	}
	else
	{
		zoom.ing = false;
	}
	p = zoom.position * (1.0f - s);

	return s;
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

static float _lineWidth = 1.0f;
void setLineWidth(float width)
{
	_lineWidth = width;
}
float getLineWidth()
{
	return _lineWidth;
}

void clearRect()
{
	glClearColor(_r, _g, _b, _a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void setClip(float x, float y, float width, float height)
{
	if (width == 0.0f || height == 0.0f)
	{
		glDisable(GL_SCISSOR_TEST);
		//glScisor(0,0,devSize.width, devSize.height);
	}

	else
	{
		glEnable(GL_SCISSOR_TEST);
		// devSize(x,y, width, height) => 
#if 0 //backBuffer 쓸때
		glScissor(x, y, width, height);
#else //backBuffer 안쓸때
		//win_border_width
		glScissor(x, y + 16, width, height);
#endif

	}
}

static float dotSize_ = 1.0f;
void setDotSize(float size)
{
	dotSize_ = size;
}
void drawDot_(float x, float y, const char* strVert, const char* strFrag)
{
	struct Dot
	{
		float position[4];
		float color[4];
	};

	float radius = dotSize_ / 2;
	Dot dot[4] =
	{
		{{-radius - 0.5f, -radius - 0.5f, 0, 1}, {_r, _g, _b, _a}},
		{{+radius + 0.5f, -radius - 0.5f, 0, 1}, {_r, _g, _b, _a}},
		{{-radius - 0.5f, +radius + 0.5f, 0, 1}, {_r, _g, _b, _a}},
		{{+radius + 0.5f, +radius + 0.5f, 0, 1}, {_r, _g, _b, _a}},
	};

	GLuint id = vtx->useProgram(strVert, strFrag);
	glBindBuffer(GL_ARRAY_BUFFER, vtx->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Dot) * 4, dot);

	GLuint attr = glGetAttribLocation(id, "position");
	glEnableVertexAttribArray(attr);
	glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(Dot), (const void*)offsetof(Dot, position));
	GLuint attrPosition = attr;
	
	attr = glGetAttribLocation(id, "color");
	glEnableVertexAttribArray(attr);
	glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(Dot), (const void*)offsetof(Dot, color));
	GLuint attrColor = attr;

	GLuint loc = glGetUniformLocation(id, "projMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixProj->d());

	matrixView->loadIdentity();
	loc = glGetUniformLocation(id, "viewMatrix");
	matrixView->push();
	matrixView->translate(x, y, 0);
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixView->d());

	// 좌표값(x, y) & 반지름 (radius)
	loc = glGetUniformLocation(id, "center");
	glUniform2f(loc, x, devSize.height - y);
	loc = glGetUniformLocation(id, "radius");
	glUniform1f(loc, radius);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vtx->vbe);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(attrPosition);
	glDisableVertexAttribArray(attrColor);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	matrixView->pop();

}

void drawDot(float x, float y)
{
	drawDot_(x, y, "dot", "dot");
}

void drawCircle(float x, float y, float radius)
{
	struct Dot
	{
		float position[4];
		float color[4];
	};

	Dot dot[4] =
	{
		{{-radius - 0.5f, -radius - 0.5f, 0, 1}, {_r, _g, _b, _a}},
		{{+radius + 0.5f, -radius - 0.5f, 0, 1}, {_r, _g, _b, _a}},
		{{-radius - 0.5f, +radius + 0.5f, 0, 1}, {_r, _g, _b, _a}},
		{{+radius + 0.5f, +radius + 0.5f, 0, 1}, {_r, _g, _b, _a}},
	};

	GLuint id = vtx->useProgram("dot", "drawCircle");
	glBindBuffer(GL_ARRAY_BUFFER, vtx->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Dot) * 4, dot);

	GLuint attr = glGetAttribLocation(id, "position");
	glEnableVertexAttribArray(attr);
	glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(Dot), (const void*)offsetof(Dot, position));
	GLuint attrPosition = attr;

	attr = glGetAttribLocation(id, "color");
	glEnableVertexAttribArray(attr);
	glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(Dot), (const void*)offsetof(Dot, color));
	GLuint attrColor = attr;

	GLuint loc = glGetUniformLocation(id, "projMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixProj->d());
	
	matrixView->loadIdentity();
	loc = glGetUniformLocation(id, "viewMatrix");
	matrixView->push();
	matrixView->translate(x, y, 0);
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixView->d());

	//좌표값(x, y) & 반지름(radius)
	loc = glGetUniformLocation(id, "center");
	glUniform2f(loc, x, devSize.height - y);
	loc = glGetUniformLocation(id, "radius");
	glUniform1f(loc, radius);
	loc = glGetUniformLocation(id, "lineWidth");
	glUniform1f(loc, _lineWidth);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vtx->vbe);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(attrPosition);
	glDisableVertexAttribArray(attrColor);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	matrixView->pop();
}
void fillCircle(float x, float y, float radius)
{
	float ds = dotSize_;

	setDotSize(2 * radius);
	drawDot(x, y);

	dotSize_ = ds;
}
static Texture** texDots = NULL;
void checkDot()
{
	if (texDots)
		return;

	texDots = new Texture * [2];

	//256 * 256
	uint8* rgba = new uint8[256 * 256 * 4];
	for (int i = 0; i < 2; i++)
	{
		if (i == 0) //Rect
		{
			memset(rgba, 0xff, sizeof(uint8) * 256 * 256 * 4);
		}
		else
		{
			iPoint center = iPointMake(127.5, 127.5);
			for (int y = 0; y < 256; y++)
			{
				for (int x = 0; x < 256; x++)
				{
					uint8* c = &rgba[y * 256 * 4 + 4 * x];
					c[0] = c[1] = c[2] = 0xff;
					c[3] = sqrtf((center.x - x) * (center.x - x) + (center.y - y) * (center.y - y)) > 127.5 ? 0 : 0xff;
				}
			}
		}
		texDots[i] = createImageWithRGBA(rgba, 256, 256);
	}
	delete rgba;
}

#define SHADER 0
void drawLine(iPoint sp, iPoint ep)
{
#if SHADER //ONLY SHADER
	float lw = _lineWidth / 2;
	float d = iPointDistance(sp, ep) / 2;

	float p[4][4] =
	{
		{-d - lw, -lw, 0, 1}, {+d + lw, -lw, 0, 1},
		{-d - lw, +lw, 0, 1}, {+d + lw, +lw, 0, 1},
	};


	GLuint id = vtx->useProgram("dot", "drawLine");
	glBindBuffer(GL_ARRAY_BUFFER, vtx->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);

	GLuint loc = glGetUniformLocation(id, "projMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixProj->d());

	loc = glGetUniformLocation(id, "viewMatrix");
	matrixView->push();
	iPoint c = (sp + ep) / 2;
	matrixView->translate(c.x, c.y, 0);
	float theta = iPointAngle(iPointMake(1, 0), iPointZero, ep - sp);
	matrixView->rotate(0, 0, 1, 360 - theta);
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixView->d());

	GLuint attr = glGetAttribLocation(id, "position");
	glEnableVertexAttribArray(attr);
	glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, 0, (const void*)0); //stride 되는 사이즈가 struct면 0아닌 struct넣기

	loc = glGetUniformLocation(id, "sp");
	//glUniform2f(loc, sp.x, devSize.height - sp.y);
	iPoint _sp = iPointMake(sp.x, devSize.height - sp.y);
	glUniform2fv(loc, 1, (float*)&_sp);

	loc = glGetUniformLocation(id, "ep");
	//glUniform2f(loc, ep.x, devSize.height - ep.y);
	iPoint _ep = iPointMake(ep.x, devSize.height - ep.y);
	glUniform2fv(loc, 1, (float*)&_ep);

	loc = glGetUniformLocation(id, "lineWidth");
	glUniform1f(loc, _lineWidth);

	loc = glGetUniformLocation(id, "color");
	glUniform4f(loc, _r, _g, _b, _a);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vtx->vbe);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(attr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	matrixView->pop();

#else// use drawImage
	checkDot();

	Texture* t = texDots[1];
	float s = getLineWidth() / (t->width);
	float degree = iPointAngle(iPointMake(1, 0), iPointZero, ep - sp);
	//왼쪽 점
	drawImage(	t, sp.x, sp.y, VCENTER | HCENTER,
				0, 0, t->width, t->height,
				s, s, 2, 360-degree);

	//오른쪽 점
	drawImage(	t, ep.x, ep.y, VCENTER | HCENTER,
				0, 0, t->width, t->height,
				s, s, 2, 180-degree);

	//사각형 부분
	t = texDots[0];

	iPoint c = sp + iPointMake((ep.x - sp.x) / 2, (ep.y - sp.y) / 2);
	float d = iPointDistance(sp, ep);

	drawImage(	t, c.x, c.y, VCENTER | HCENTER,
				0, 0, d, t->height,
				1.0f, s, 2, 360 - degree);
	
#endif

}
void drawLine(float x0, float y0, float x1, float y1)
{
	iPoint p0 = iPointMake(x0, y0);
	iPoint p1 = iPointMake(x1, y1);
	drawLine(p0, p1);
}

void drawRect(iRect rt, float radius)
{
	drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}

#define RECT_SHADER 0
void drawRect(float x, float y, float width, float height, float radius)
{
#if RECT_SHADER
	//가운데 점 Center를 기준으로 뿌리는 영역 4개를 설정하기
	float p[4][4] =
	{
		{ -width / 2 - 0.5, -height / 2 - 0.5f, 0, 1 }, { width / 2 + 0.5, -height / 2 - 0.5f, 0, 1 },
		{ -width / 2 - 0.5, +height / 2 + 0.5f, 0, 1 }, { width / 2 + 0.5, +height / 2 + 0.5f, 0, 1 },
};

	GLuint id = vtx->useProgram("dot", "drawRect");
	glBindBuffer(GL_ARRAY_BUFFER, vtx->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);


	GLuint attr = glGetAttribLocation(id, "position");
	glEnableVertexAttribArray(attr);
	glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, 0, (const void*)0);

	GLuint loc = glGetUniformLocation(id, "projMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixProj->d());

	loc = glGetUniformLocation(id, "viewMatrix");
	matrixView->push();
	matrixView->translate(x + width / 2, y + height / 2, 0); //회전을 생각해서 가운데 정점을 이용
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixView->d());

	loc = glGetUniformLocation(id, "rect");
	glUniform4f(loc, x + width / 2, devSize.height - (y + height / 2), width / 2, height / 2);
	loc = glGetUniformLocation(id, "radius");
	glUniform1f(loc, radius);
	loc = glGetUniformLocation(id, "lineWidth");
	glUniform1f(loc, _lineWidth);
	loc = glGetUniformLocation(id, "color");
	glUniform4f(loc, _r, _g, _b, _a);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vtx->vbe);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(attr);
	matrixView->pop();
#else
	//#issue! strange shape
	checkDot();

	Texture* t = texDots[1];
	iPoint c = iPointMake(x + width / 2, y + height / 2);
	float lw = getLineWidth() / t->width;
	
	//rect-circle (라운딩)
	iPoint p1 = iPointMake(c.x - width / 2 + radius, c.y - height / 2 + radius);
	iPoint p2 = iPointMake(c.x + width / 2 - radius, c.y - height / 2 + radius);
	iPoint p3 = iPointMake(c.x - width / 2 + radius, c.y + height / 2 - radius);
	iPoint p4 = iPointMake(c.x + width / 2 - radius, c.y + height / 2 - radius);
	
	float degree = iPointAngle(iPointMake(1, 0), iPointZero, p1 - p2);
	float s = (radius*2 / t->width);

	//
	//dot
	//
	drawImage(t, p1.x, p1.y, VCENTER | HCENTER,
		0, 0, t->width, t->height,
		s, s, 2, 360 - degree);

	drawImage(t, p2.x, p2.y, VCENTER | HCENTER,
		0, 0, t->width, t->height,
		s, s, 2, 360 - degree);

	drawImage(t, p3.x, p3.y, VCENTER | HCENTER,
		0, 0, t->width, t->height,
		s, s, 2, 360 - degree);

	drawImage(t, p4.x, p4.y, VCENTER | HCENTER,
		0, 0, t->width, t->height,
		s, s, 2, 360 - degree);

	//
	//line
	//
	t = texDots[0];
	iPoint c1 = p1 + iPointMake((p2.x - p1.x) / 2, (p2.y - p1.y) / 2); //p1 - p2
	iPoint c2 = p4 + iPointMake((p2.x - p4.x) / 2, (p2.y - p4.y) / 2); //p2 - p4
	iPoint c3 = p3 + iPointMake((p4.x - p3.x) / 2, (p4.y - p3.y) / 2); //p4 - p3
	iPoint c4 = p3 + iPointMake((p1.x - p3.x) / 2, (p1.y - p3.y) / 2); //p1 - p3

	float d = iPointDistance(p1, p2); //width : c1, c3
	float d2 = iPointDistance(p1, p3); //height : c2, c4

	drawImage(t, c1.x, c1.y, VCENTER | HCENTER,
		0, 0, d, t->height,
		1.0f, lw, 2, 360 - degree);

	drawImage(t, c3.x, c3.y, VCENTER | HCENTER,
		0, 0, d, t->height,
		1.0f, lw, 2, 360 - degree);

	drawImage(t, c2.x, c2.y, VCENTER | HCENTER,
		0, 0, t->width, d2,
		lw, 1.0f, 2, 360 - degree);

	drawImage(t, c4.x, c4.y, VCENTER | HCENTER,
		0, 0, t->width, d2,
		lw, 1.0f, 2, 360 - degree);
#endif
}

void fillRect(iRect rt, float radius)
{
	fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}
void fillRect(float x, float y, float width, float height, float radius)
{
#if RECT_SHADER
	//가운데 점 Center를 기준으로 뿌리는 영역 4개를 설정하기
	float p[4][4] =
	{
		{ -width / 2 - 0.5, -height / 2 - 0.5f, 0, 1 }, { width / 2 + 0.5, -height / 2 - 0.5f, 0, 1 },
		{ -width / 2 - 0.5, +height / 2 + 0.5f, 0, 1 }, { width / 2 + 0.5, +height / 2 + 0.5f, 0, 1 },
	};

	GLuint id = vtx->useProgram("dot", "fillRect");
	glBindBuffer(GL_ARRAY_BUFFER, vtx->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);


	GLuint attr = glGetAttribLocation(id, "position");
	glEnableVertexAttribArray(attr);
	glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, 0, (const void*)0);

	GLuint loc = glGetUniformLocation(id, "projMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixProj->d());

	loc = glGetUniformLocation(id, "viewMatrix");
	matrixView->push();
	matrixView->translate(x + width / 2, y + height / 2, 0); //회전을 생각해서 가운데 정점을 이용
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixView->d());

	loc = glGetUniformLocation(id, "rect");
	glUniform4f(loc, x + width / 2, devSize.height - (y + height / 2), width / 2, height / 2);
	loc = glGetUniformLocation(id, "radius");
	glUniform1f(loc, radius);
	loc = glGetUniformLocation(id, "color");
	glUniform4f(loc, _r, _g, _b, _a);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vtx->vbe);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(attr);
	matrixView->pop();
#else
	checkDot();

	Texture* t = texDots[0];
	float s = getLineWidth() / (t->width);
	iPoint sp = iPointMake(x - width / 2, y - height / 2);
	iPoint ep = iPointMake(x + width / 2, y - height / 2);
	float degree = iPointAngle(iPointMake(1, 0), iPointZero, ep - sp);

	//rect
	
	float rw = (width-radius) / t->width;
	float rh = (height-radius) / t->height;
	iPoint c = iPointMake(x + width / 2, y + height / 2);
	drawImage(t, c.x, c.y, VCENTER | HCENTER,
		0, 0, t->width, t->height,
		rw, rh, 2, 180 - degree);

	//rect-circle (라운딩)
	t = texDots[1];
	s = radius*2 / t->width;
	
	iPoint p1 = iPointMake(c.x - width / 2 + radius, c.y - height / 2 + radius);
	iPoint p2 = iPointMake(c.x + width / 2 - radius, c.y - height / 2 + radius);
	iPoint p3 = iPointMake(c.x - width / 2 + radius, c.y + height / 2 - radius);
	iPoint p4 = iPointMake(c.x + width / 2 - radius, c.y + height / 2 - radius);

	//
	//dot
	//
	drawImage(t, p1.x, p1.y, VCENTER | HCENTER,
		0, 0, t->width, t->height,
		s, s, 2, 360 - degree);

	drawImage(t, p2.x, p2.y, VCENTER | HCENTER,
		0, 0, t->width, t->height,
		s, s, 2, 360 - degree);

	drawImage(t, p3.x, p3.y, VCENTER | HCENTER,
		0, 0, t->width, t->height,
		s, s, 2, 360 - degree);

	drawImage(t, p4.x, p4.y, VCENTER | HCENTER,
		0, 0, t->width, t->height,
		s, s, 2, 360 - degree);

	//
	//line
	//
	t = texDots[0];
	iPoint c1 = p1 + iPointMake((p2.x - p1.x) / 2, (p2.y - p1.y) / 2); //p1 - p2
	iPoint c2 = p4 + iPointMake((p2.x - p4.x) / 2, (p2.y - p4.y) / 2); //p2 - p4
	iPoint c3 = p3 + iPointMake((p4.x - p3.x) / 2, (p4.y - p3.y) / 2); //p4 - p3
	iPoint c4 = p3 + iPointMake((p1.x - p3.x) / 2, (p1.y - p3.y) / 2); //p1 - p3

	float d = iPointDistance(p1, p2); //width : c1, c3
	float d2 = iPointDistance(p1, p3); //height : c2, c4

	drawImage(t, c1.x, c1.y, VCENTER | HCENTER,
		0, 0, d, t->height,
		1.0f, s, 2, 360 - degree);

	drawImage(t, c3.x, c3.y, VCENTER | HCENTER,
		0, 0, d, t->height,
		1.0f, s, 2, 360 - degree);

	drawImage(t, c2.x, c2.y, VCENTER | HCENTER,
		0, 0, t->width, d2,
		s, 1.0f, 2, 360 - degree);
	
	drawImage(t, c4.x, c4.y, VCENTER | HCENTER,
		0, 0, t->width, d2,
		s, 1.0f, 2, 360 - degree);


#endif
}

uint32 nextPot(uint32 x)
{
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	return x + 1;
}

void copyBmp2RGBA(uint32* src, int stride, int x, int y, int w, int h,
				  uint32* dst, int dw, int dh)
{
	int potWidth = nextPot(dw);
	int potHeight = nextPot(dh);
	memset(dst, 0x00, sizeof(uint32) * potWidth * potHeight);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			uint8* d = (uint8*)&dst[potWidth * j + i];
			uint8* s = (uint8*)&src[stride * (y + j) + (x + i)];
			d[0] = s[2]; // r
			d[1] = s[1]; // g
			d[2] = s[0]; // b
			d[3] = s[3]; // a
		}
	}
}

void copyBmp2RGBABgAlpha(uint32* src, int stride, int x, int y, int w, int h,
						uint32* dst, int dw, int dh)
{
	int potWidth = nextPot(dw);
	int potHeight = nextPot(dh);
	memset(dst, 0x00, sizeof(uint32) * potWidth * potHeight);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			uint8* d = (uint8*)&dst[potWidth * j + i];
			if (src[stride * (y + j) + (x + i)] == src[0])
			{
				d[3] = 0;
				continue;
			}
			uint8* s = (uint8*)&src[stride * (y + j) + (x + i)];
			d[0] = s[2]; // r
			d[1] = s[1]; // g
			d[2] = s[0]; // b
			d[3] = s[3]; // a
		}
	}
}



Texture* createImageWithRGBA(uint8* rgba, int width, int height)
{
	int potWidth = nextPot(width);
	int potHeight = nextPot(height);

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	setTexture();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	glBindTexture(GL_TEXTURE_2D, 0);

	Texture* tex = new Texture;
	tex->texID = texID;
	tex->width = width;
	tex->height = height;
	tex->potWidth = potWidth;
	tex->potHeight = potHeight;
	tex->retainCount = 1;
#if CHECK_TEXTURE
	texture_num++;
#endif
	return tex;
}

Texture* createTexture(int width, int height, bool rgba32f)
{
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	setTexture();
	if (rgba32f == false)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}
	else
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	}
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, 0);

	Texture* tex = new Texture;
	tex->texID = texID;
	tex->width = width;
	tex->height = height;
	tex->potWidth = width;
	tex->potHeight = height;
	tex->retainCount = 1;
#if CHECK_TEXTURE
	texture_num++;
#endif

	return tex;
}

#if CHECK_TEXTURE
	int texture_num = 0;
#endif

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
	int potW = nextPot(w);
	int potH = nextPot(h);
	uint32* pixel = new uint32[potW * potH];

	for (int j = 0; j < numY; j++)
	{
		for (int i = 0; i < numX; i++)
		{
			copyBmp2RGBA(rgba, stride, w * i, h * j, w, h, pixel, w, h);
			texs[numX * j + i] = createImageWithRGBA((uint8*)pixel, w, h);
		}
	}

	delete pixel;

	// unlock
	bmp->UnlockBits(&bmpData);

	return texs;
}

Texture* createImage(const char* szFormat, ...)
{
	char szText[1024];
	va_start_end(szText, szFormat);

	wchar_t* ws = utf8_to_utf16(szText);
	Bitmap* bmp = Bitmap::FromFile(ws, false);
	delete ws;

	int width = bmp->GetWidth();
	int height = bmp->GetHeight();
	Rect rect(0, 0, width, height);
	BitmapData bmpData;
	bmp->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

	int stride = bmpData.Stride / 4;
	uint32* rgba = (uint32*)bmpData.Scan0;

	int potWidth = nextPot(width);
	int potHeight = nextPot(height);
	uint32* pixel = new uint32[potWidth * potHeight];
	copyBmp2RGBA(rgba, stride, 0, 0, width, height,
		pixel, width, height);

	Texture* tex = createImageWithRGBA((uint8*)pixel, width, height);
	delete pixel;

	bmp->UnlockBits(&bmpData);

	return tex;
}

Texture** createImageAlphaDivide(int numX, int numY, const char* szFormat, ...)
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
	int potW = nextPot(w);
	int potH = nextPot(h);
	uint32* pixel = new uint32[potW * potH];

	for (int j = 0; j < numY; j++)
	{
		for (int i = 0; i < numX; i++)
		{
			copyBmp2RGBABgAlpha(rgba, stride, w * i, h * j, w, h, pixel, w, h);
			texs[numX * j + i] = createImageWithRGBA((uint8*)pixel, w, h);
		}
	}

	delete pixel;

	// unlock
	bmp->UnlockBits(&bmpData);

	return texs;
}

Texture* createImageAlpha(const char* szFormat, ...)
{
	char szText[1024];
	va_start_end(szText, szFormat);

	wchar_t* ws = utf8_to_utf16(szText);
	Bitmap* bmp = Bitmap::FromFile(ws, false);
	delete ws;

	int width = bmp->GetWidth();
	int height = bmp->GetHeight();
	Rect rect(0, 0, width, height);
	BitmapData bmpData;
	bmp->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

	int stride = bmpData.Stride / 4;
	uint32* rgba = (uint32*)bmpData.Scan0;

	int potWidth = nextPot(width);
	int potHeight = nextPot(height);
	uint32* pixel = new uint32[potWidth * potHeight];
	copyBmp2RGBABgAlpha(rgba, stride, 0, 0, width, height,
		pixel, width, height);

	Texture* tex = createImageWithRGBA((uint8*)pixel, width, height);
	delete pixel;

	bmp->UnlockBits(&bmpData);

	return tex;
}

void freeImage(Texture* tex)
{
	if (tex->retainCount > 1)
	{
		tex->retainCount--;
		return;
	}
	glDeleteTextures(1, &tex->texID);
	delete tex;
}

void drawImage(Texture* tex, float x, float y, int anc,
	float ix, float iy, float iw, float ih,
	float rx, float ry,
	int xyz, float degree, int reverse)
{
	//float w = tex->width * rx, h = tex->height * ry;
	float w = iw * rx, h = ih * ry;

	switch (anc) {
	case TOP | LEFT:								break;
	case TOP | RIGHT:		x -= w;					break;
	case TOP | HCENTER:		x -= w / 2;				break;

	case BOTTOM | LEFT:					y -= h;		break;
	case BOTTOM | RIGHT:	x -= w;		y -= h;		break;
	case BOTTOM | HCENTER:	x -= w / 2;	y -= h;		break;

	case VCENTER | LEFT:				y -= h / 2;	break;
	case VCENTER | RIGHT:	x -= w;		y -= h / 2;	break;
	case VCENTER | HCENTER:	x -= w / 2;	y -= h / 2;	break;
	}
	iPoint dstPoint[4] = {
		{0, 0},	{w, 0},
		{0, h},	{w, h}
	};
	if (reverse == REVERSE_NONE);
	else if (reverse == REVERSE_WIDTH)
	{
		float t = dstPoint[0].x;
		dstPoint[0].x = dstPoint[1].x;
		dstPoint[1].x = t;

		dstPoint[2].x = dstPoint[0].x;
		dstPoint[3].x = dstPoint[1].x;
	}
	else if (reverse == REVERSE_HEIGHT)
	{
		float t = dstPoint[0].y;
		dstPoint[0].y = dstPoint[2].y;
		dstPoint[2].y = t;

		dstPoint[1].y = dstPoint[0].y;
		dstPoint[3].y = dstPoint[2].y;
	}

	matrixView->loadIdentity();
	matrixView->push();
	matrixView->translate(x, y, 0);


	if (degree)
	{
		iPoint t = iPointMake(x + w / 2, y + h / 2);
		if (xyz == 0)// x
		{
			matrixView->translate(w / 2, h / 2, 0);
			matrixView->rotate(1, 0, 0, degree);
			matrixView->translate(-w / 2, -h / 2, 0);
		}
		else if (xyz == 1)// y
		{
			matrixView->translate(w / 2, h / 2, 0);
			matrixView->rotate(0, 1, 0, degree);
			matrixView->translate(-w / 2, -h / 2, 0);
		}
		else// if (xyz == 2)// z
		{
			matrixView->translate(w / 2, h / 2, 0);
			matrixView->rotate(0, 0, 1, degree);
			matrixView->translate(-w / 2, -h / 2, 0);
		}
	}



	// 0,0   1,0
	//
	// 0,1   1,1
	iPoint st[4] = {
		{ix / tex->potWidth, iy / tex->potHeight},			{(ix + iw) / tex->potWidth, iy / tex->potHeight},
		{ix / tex->potWidth, (iy + ih) / tex->potHeight},	{(ix + iw) / tex->potWidth, (iy + ih) / tex->potHeight},
	};

	iColor4f c[4] = {
		{_r, _g, _b, _a},	{_r, _g, _b, _a},
		{_r, _g, _b, _a},	{_r, _g, _b, _a},
	};

	//3.x ver
	GLuint id = vtx->useProgram("alpha", "alpha");
	VertexInfo vi[4];
	for (int i = 0; i < 4; i++)
	{
		memcpy(vi[i].position, &dstPoint[i], sizeof(iPoint));
		vi[i].position[2] = 0; // z
		vi[i].position[3] = 1; // w
		memcpy(&vi[i].color, &c[i], sizeof(iColor4f));
		memcpy(&vi[i].uv, &st[i], sizeof(iPoint));
	}
	vtx->bufferSubData(vi, 1, 1);
	GLuint attrP = vtx->enableVertexAttrArray("position", (const void*)offsetof(VertexInfo, position), sizeof(VertexInfo), 4, GL_FLOAT);
	GLuint attrC = vtx->enableVertexAttrArray("color", (const void*)offsetof(VertexInfo, color), sizeof(VertexInfo), 4, GL_FLOAT);
	GLuint attrT = vtx->enableVertexAttrArray("texcoord", (const void*)offsetof(VertexInfo, uv), sizeof(VertexInfo), 2, GL_FLOAT);

	vtx->setUniformMat("projMatrix", matrixProj);
	vtx->setUniformMat("viewMatrix", matrixView);
	vtx->setUniform1i("tex", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->texID);

	vtx->drawElements();

	vtx->disableVertexAttrArray(attrP);
	vtx->disableVertexAttrArray(attrC);
	vtx->disableVertexAttrArray(attrT);

	vtx->bufferSubData(NULL, 0, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	matrixView->pop();

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
static float stringLineHeight = 30.0f;
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
	int strLen = strlen(str);
	stringName = new char[strLen + 1];
#if 0
	strcpy(stringName, str);
#else
	memcpy(stringName, str, strLen);
	stringName[strLen] = 0;
#endif
}

float getStringSize()
{
	return stringSize;
}
void setStringSize(float size)
{
	stringSize = size;
}

void setStringLineHeight(float height)
{
	stringLineHeight = height;
}

float getStringLineHeight()
{
	return stringLineHeight;
}

void getStringRGBA(float& r, float& g, float& b, float& a)
{
	r = stringR;
	g = stringG;
	b = stringB;
	a = stringA;
}

void setStringRGBA(float r, float g, float b, float a)
{
	stringR = r;
	stringG = g;
	stringB = b;
	stringA = a;
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

iFont* loadFont(const char* strOTFTTF, float height, const char* strUse)
{
	iFont* f = new iFont;
	f->height = height;
	f->interval = 1;
	f->texs = new Texture * [256];
	memset(f->texs, 0x00, sizeof(Texture*) * 256);

	setStringName(strOTFTTF);
	setStringSize(height);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	iGraphics* g = iGraphics::share();

	//int i, len = strlen(strUse);
	//for (i = 0; i < len; i++)
	for (int i = 0; strUse[i]; i++)
	{
		char c = strUse[i];
		if (f->texs[c]) continue;

		if (c == ' ')
		{
			g->init(height * 0.6, height);

			f->texs[c] = g->getTexture();
			continue;
		}
		else if (c == ',')
		{
			g->init(height * 0.6, height);

			iRect rt = iGraphics::rectOfString(",");
			g->drawString(0, height - rt.size.height,
				TOP | LEFT, ",");

			f->texs[c] = g->getTexture();
			continue;
		}

		char str[2] = { c, 0 };
		iRect rt = iGraphics::rectOfString(str);
		g->init(rt.size);

		g->drawString(0, 0, TOP | LEFT, str);

		f->texs[c] = g->getTexture();
	}

	return f;
}

void freeFont(iFont* font)
{
	for (int i = 0; i < 256; i++)
	{
		if (font->texs[i])
			freeImage(font->texs[i]);
	}
	delete font->texs;
	delete font;
}

iFont* fontCurr;
void setFont(iFont* font)
{
	fontCurr = font;
}

void drawString(float x, float y, int anc, const char* szFormat, ...)
{
	char szText[128];
	va_start_end(szText, szFormat);
	drawString(x, y, 1.0f, 1.0f, anc, szText);
}

void drawString(float x, float y, float sx, float sy, int anc, const char* szFormat, ...)
{
	char szText[128];
	va_start_end(szText, szFormat);

	float h = fontCurr->height;
	float w = 0;
	int i, len = strlen(szText) - 1;
	for (i = 0; i < len; i++)
		w += (fontCurr->texs[szText[i]]->width + fontCurr->interval);
	w += fontCurr->texs[szText[i]]->width;

	switch (anc) {
	case TOP | LEFT:								break;
	case TOP | RIGHT:		x -= w;					break;
	case TOP | HCENTER:		x -= w / 2;				break;

	case BOTTOM | LEFT:					y -= h;		break;
	case BOTTOM | RIGHT:	x -= w;		y -= h;		break;
	case BOTTOM | HCENTER:	x -= w / 2;	y -= h;		break;

	case VCENTER | LEFT:				y -= h / 2;	break;
	case VCENTER | RIGHT:	x -= w;		y -= h / 2;	break;
	case VCENTER | HCENTER:	x -= w / 2;	y -= h / 2;	break;
	}

	Texture* tex;
	len++;
#if 0
	w = 0;
	for (i = 0; i < len; i++)
	{
		tex = fontCurr->texs[szText[i]];
		drawImage(tex, x + w * sx, y, TOP | LEFT);
		w += (tex->width + fontCurr->interval);
	}
#else 
	for (i = 0; i < len; i++)
	{
		tex = fontCurr->texs[szText[i]];
		drawImage(tex, x, y, TOP | LEFT);
		x += (tex->width + fontCurr->interval);
	}
#endif
}


iRect rectOfString(const char* szFormat, ...)
{
	return iRectMake(0, 0, 0, 0);
}

char* loadFile(const char* fileName, int& length)
{
	FILE* pf = fopen(fileName, "rb");
	if (!pf)
		return NULL;

	fseek(pf, 0, SEEK_END);
	length = ftell(pf);
	char* buf = new char[length + 1];
	fseek(pf, 0, SEEK_SET);
	fread(buf, sizeof(char), length, pf);
	buf[length] = 0;

	fclose(pf);

	return buf;
}

void saveFile(const char* fileName, char* buf, int length)
{
	FILE* pf = fopen(fileName, "wb");
	fwrite(buf, sizeof(char), length, pf);
	fclose(pf);
}

