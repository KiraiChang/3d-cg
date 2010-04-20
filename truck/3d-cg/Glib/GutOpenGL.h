#ifndef _GUTOPENGL_H_
#define _GUTOPENGL_H_

#include "GutDefs.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

//--------------device init--------------------------------

bool GutInitGraphicsDeviceOpenGL(GutDeviceSpec *spec=NULL);
static bool SetPixelFormat(HDC hdc);

//--------------device operation---------------------------

void GutSwapBuffersOpenGL(void);

//--------------resource release--------------------------

bool GutReleaseGraphicsDeviceOpenGL(void);


#endif // _GUTOPENGL_H_