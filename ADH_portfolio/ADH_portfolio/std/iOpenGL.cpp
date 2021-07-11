#include "iOpenGL.h"

#include "iStd.h"

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

	printf("Status: Using GL %s, GLEW %s\n",
		glGetString(GL_VERSION), glewGetString(GLEW_VERSION));
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Error: %s\n", glewGetErrorString(err));
		return;
	}

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#if 0 //for Android
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#else
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_SRC);
#endif

	setGLBlend(GLBlendAlpha);
	glEnable(GL_BLEND);
	
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glShadeModel(GL_SMOOTH);

	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);

	setTextureParms(TextureWrapClamp, TextureFilterLinear);
}

void freeOpenGL()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
}

static TextureWrap textureWrap = TextureWrapClamp;
static TextureFilter textureFilter = TextureFilterLinear;

void setTextureParms(TextureWrap wrap, TextureFilter filter)
{
	textureWrap = wrap;
	textureFilter = filter;
}
void applyTextureParms()
{
	GLenum e = (textureFilter == TextureFilterLinear ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, e);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, e);
	e = (textureWrap == TextureWrapClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, e);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, e);
}

void readyOpenGL()
{
	glViewport(viewport.origin.x, viewport.origin.y,
		viewport.size.width, viewport.size.height);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, devSize.width, devSize.height, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static GLBlend _blend = GLBlendAlpha;
void setGLBlend(GLBlend blend)
{
	switch (_blend = blend) {
		// dst = dst * (1.0f - src.a) + src * src.a
	case GLBlendAlpha: //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
			GL_ONE, GL_ONE_MINUS_SRC_ALPHA); break;
		// dst = dst * 1.0f + src * src.a
	case GLBlendLight: glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;

	case GLBlendMultiplied:
		//glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA,
		//    GL_ONE, GL_ONE_MINUS_SRC_ALPHA); break;
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); break;
		break;
		//default: break;
	}
}

GLBlend getGLBlend()
{
	return _blend;

	setRGBA(1, 0, 1, 0.3f);
}

