#include "GutWin32.h"
#include "..\Utility\Gut.h"

//##############################################################################
//###########################device getter setter#####################################
//##############################################################################

void GutSetWindowHandleWin32(HWND hWnd)
{
	g_hWnd = hWnd;
}

HWND GutGetWindowHandleWin32(void)
{
	return g_hWnd;
}

HINSTANCE GutGetWindowInstanceWin32(void)
{
	return g_hInstance;
}

void GutGetWindowSize(int &w, int &h)
{
	w = g_iWindowPosWidth;
	h = g_iWindowPosHeight;
}

//##############################################################################
//##################################device init#####################################
//##############################################################################


bool GutCreateWindow(int x, int y, int width, int height, const char *title)
{
	WNDCLASS window_class;
	memset(&window_class, 0, sizeof(WNDCLASS));
	//設定視窗型態
	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	//設定訊息處理函式為WndProc
	window_class.lpfnWndProc = WndProc;
	//取的目前執行的程式
	window_class.hInstance = GetModuleHandle(NULL);
	//取得內定的滑鼠座標
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	//背景畫面設為黑色
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//給定一個註冊視窗的名子
	window_class.lpszClassName = title;
	//跟作業系統註冊一個新的視窗
	if(FALSE == RegisterClass(&window_class))
		return FALSE;

	DWORD window_style;

	if(g_GutCallBack.OnSize)//如果有處理視窗縮放的訊息,才容許滑鼠改變視窗大小
		window_style = WS_OVERLAPPEDWINDOW;
	else
		window_style = WS_BORDER | WS_SYSMENU;

	//計算視窗的邊線會占掉的點數,必要時增加視窗大小
	RECT window_rect;
	SetRect(&window_rect, x, y, x+width, y+height);
	AdjustWindowRect(&window_rect, window_style, FALSE);

	g_iWindowPosWidth = width;
	g_iWindowPosHeight = height;

	//準備完成,開啟一個視窗
	HWND window_handle = CreateWindowEx(
		WS_EX_APPWINDOW,
		title,
		title,
		window_style,
		window_rect.left,
		window_rect.top,
		window_rect.right - window_rect.left,
		window_rect.bottom - window_rect.top,
		NULL,
		NULL,
		window_class.hInstance,
		NULL
		);
	if(NULL == window_handle)
		return FALSE;

	//記錄下用來代表視窗跟目前執行程式的物件,以後會再用到
	g_hWnd = window_handle;
	g_hInstance = window_class.hInstance;
	//顯示視窗
	ShowWindow(window_handle, SW_SHOWNORMAL);
	SetActiveWindow(window_handle);

	return TRUE;
}

//##############################################################################
//##########################message proc##########################################
//##############################################################################

//訊息處理
static LRESULT WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	case WM_SIZE:
		{
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);
			g_iWindowPosHeight = h;
			g_iWindowPosWidth = w;
			if(g_GutCallBack.OnSize && GUT_UNKNOWN != GutGetGraphicsDeviceType())
				g_GutCallBack.OnSize(w,h);
			break;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			if(g_GutCallBack.OnPaint)
				g_GutCallBack.OnPaint();
			break;
		}
	default:
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
			break;
		}
	}
	return FALSE;
}

//檢查Windows作業系統傳來的訊息
bool GutProcessMessage(void)
{
	MSG	msg;
	//看看有沒有任何訊息`
	if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
	{
		// 有訊息的話, 就把它送給訊息處理函式.
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if ( msg.message==WM_QUIT )
		{
			//收到關閉視窗的訊息會傳回false來結束迴圈.
			return FALSE;
		}
	}
	return TRUE;
}
