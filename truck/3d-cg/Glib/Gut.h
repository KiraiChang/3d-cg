#ifndef _GUT_H_
#define _GUT_H_

#include <windows.h>
#include <GL/gl.h>
#include <d3d9.h>

#include "GutDef.h"
#include "GutOpenGL.h"

extern GutCallBack g_GutCallBack;
bool GutCreateWindow(int x, int y, int width, int height, const char *title = "Gut Application");
bool GutProcessMessage(void);
//bool GutReleaseGraphicsDevice(void);



#endif //_GUT_H_