#include <windows.h>
#include <stdio.h>

//#include "glew.h"
//#include "wglew.h"
#include <GL/gl.h>

#include "GutOpenGL.h"
#include "GutWin32.h"

//#############################################################
//########################device init#############################
//#############################################################

bool GutInitGraphicsDeviceOpenGL(GutDeviceSpec *spec)
{
	//開啟視窗時就已取得這個用來代表視窗的指標
	HWND hWnd = GutGetWindowHandleWin32();
	if(NULL == hWnd)
		return FALSE;
	//取得視窗的軟體裝置
	g_hDC = GetDC(hWnd);
	if(!SetPixelFormat(g_hDC))
	{
		ReleaseDC(hWnd, g_hDC);
		return FALSE;
	}
	return TRUE;
}

static bool SetPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));

	//設定OpenGL的裝置格式
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cDepthBits = 24;//24bits z-buffer
	pfd.cStencilBits = 8;//8bits stencil buffer
	pfd.iLayerType = PFD_MAIN_PLANE;//main layout

	//檢查硬體是否有支援上面的格式
	int pixelformat = ChoosePixelFormat(hdc, &pfd);
	if(FALSE == pixelformat)
		return FALSE;
	if(FALSE == SetPixelFormat(hdc, pixelformat, &pfd))
		return FALSE;

	//建立OPENGL裝置
	g_hGLRC = wglCreateContext(g_hDC);
	//使用OPENGL裝置
	wglMakeCurrent(g_hDC, g_hGLRC);

	return TRUE;
}

//#############################################################
//#####################device opertion#############################
//#############################################################


void GutSwapBuffersOpenGL(void)
{
	SwapBuffers(g_hDC);
}


//#############################################################
//####################resource release#############################
//#############################################################

bool GutReleaseGraphicsDeviceOpenGL(void)
{
	HWND hWnd = GutGetWindowHandleWin32();

	wglMakeCurrent(g_hDC, NULL);
	wglDeleteContext(g_hGLRC);
	g_hGLRC = NULL;

	ReleaseDC(hWnd, g_hDC);
	g_hDC = NULL;

	return true;
}
