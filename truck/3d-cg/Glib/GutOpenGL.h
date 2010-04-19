#ifndef _GUTOPENGL_H_
#define _GUTOPENGL_H_

#include "GutDef.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

//--------------global object--------------------------------

static HDC g_hDC = NULL;
static HGLRC g_hGLRC = NULL;

//--------------device init--------------------------------

bool GutInitGraphicsDeviceOpenGL(GutDeviceSpec *spec=NULL);
static bool SetPixelFormat(HDC hdc);

//--------------device operation---------------------------

void GutSwapBuffersOpenGL(void);

//--------------resource release--------------------------

bool GutReleaseGraphicsDeviceOpenGL(void);


#endif // _GUTOPENGL_H_