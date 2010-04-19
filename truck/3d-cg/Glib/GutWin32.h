#ifndef _GUT_WIN32_H_
#define _GUT_WIN32_H_

#include <windows.h>

static HWND g_hWnd = NULL;
static HINSTANCE g_hInstance = NULL;
static HANDLE g_ThreadHandle = NULL;
static unsigned long g_ThreadID = 0;

static int g_iWindowPosX = 100;
static int g_iWindowPosY = 100;
static int g_iWindowPosWidth = 512;
static int g_iWindowPosHeight = 512;

void GutSetWindowHandleWin32(HWND hWnd);
HWND GutGetWindowHandleWin32(void);
HINSTANCE GutGetWindowInstanceWin32(void);
void GutGetWindowSize(int &w, int &h);

static LRESULT WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
bool GutCreateWindow(int x, int y, int width, int height, const char *title);

#endif //_GUT_WIN32_H_