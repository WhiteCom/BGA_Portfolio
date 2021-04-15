#pragma once

#include "iGraphics.h"

#include <GL/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "OpenGL32.lib")


//void loadOpenGL(HDC hDC);
void freeOpenGL();

enum TextureWrap {
	TextureWrapClamp = 0,
	TextureWrapRepeat
};

enum TextureFilter {
	TextureFilterNearest = 0,
	TextureFilterLinear,
};

void setTextureParms(TextureWrap wrap, TextureFilter filter);
void applyTextureParms();

void readyOpenGL();
