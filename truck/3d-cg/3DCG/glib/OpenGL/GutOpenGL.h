#ifndef _GUTOPENGL_H_
#define _GUTOPENGL_H_

#include "..\Utility\GutDefs.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

//---------------shader tool----------------------------

GLuint GutLoadVertexProgramOpenGL_ASM(const char *filename);
GLuint GutLoadFragmentProgramOpenGL_ASM(const char *filename);

void GutReleaseVertexProgramOpenGL(GLuint shader_id);
void GutReleaseFragmentProgramOpenGL(GLuint shader_id);

//--------------device init--------------------------------

bool GutInitGraphicsDeviceOpenGL(GutDeviceSpec *spec=NULL);
static bool SetPixelFormat(HDC hdc);

//--------------device operation---------------------------

void GutSwapBuffersOpenGL(void);

//--------------resource release--------------------------

bool GutReleaseGraphicsDeviceOpenGL(void);


#endif // _GUTOPENGL_H_