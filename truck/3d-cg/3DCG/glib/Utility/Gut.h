#ifndef _GUT_H_
#define _GUT_H_

#include <windows.h>
#include <GL/gl.h>
#include <d3d9.h>

#include "GutDefs.h"
#include "..\OpenGL\GutOpenGL.h"
#include "..\DX9\GutDX9.h"
#include "..\DX10\GutDX10.h"

#include "..\math\Vector4.h"
#include "..\math\Matrix4x4.h"

bool GutCreateWindow(int x, int y, int width, int height, const char *title = "Gut Application");//from gutwin32
//--------------device init--------------------------------

bool GutInitGraphicsDevice(const char *device);
GutEnum GutGetGraphicsDeviceType(const char *device = NULL);

//--------------device operation---------------------------

Matrix4x4 GutMatrixLookAtRH(Vector4 &eye, Vector4 &lookat, Vector4 &up);

Matrix4x4 GutMatrixOrthoRH_OpenGL(float w, float h, float z_near, float z_far);
Matrix4x4 GutMatrixOrthoRH_DirectX(float w, float h, float z_near, float z_far);

Matrix4x4 GutMatrixPerspectiveRH_OpenGL(float fovy, float aspect, float z_near, float z_far);
Matrix4x4 GutMatrixPerspectiveRH_DirectX(float fovy, float aspect, float z_near, float z_far);

//------------shader tool-------------------------------

void GutSetShaderPath(const char *path);
const char *GutGetShaderPath(void);

//------------message proc-------------------------------
extern GutCallBack g_GutCallBack;
bool GutProcessMessage(void);

//--------------resource release--------------------------
bool GutReleaseGraphicsDevice(void);



#endif //_GUT_H_