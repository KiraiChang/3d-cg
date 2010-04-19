#ifndef _GUTOPENGL_H_
#define _GUTOPENGL_H_

#include "GutDef.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

bool GutInitGraphicsDeviceOpenGL(GutDeviceSpec *spec=NULL);
static bool SetPixelFormat(HDC hdc);

void GutSwapBuffersOpenGL(void);
bool GutReleaseGraphicsDeviceOpenGL(void);


#endif // _GUTOPENGL_H_