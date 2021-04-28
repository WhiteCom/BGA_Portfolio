#include "iOpenGL.h"

HGLRC hRC;

void loadOpenGL(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0x00, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	int pixelformat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelformat, &pfd);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
}

void freeOpenGL()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
}

