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
	//�]�w�������A
	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	//�]�w�T���B�z�禡��WndProc
	window_class.lpfnWndProc = WndProc;
	//�����ثe���檺�{��
	window_class.hInstance = GetModuleHandle(NULL);
	//���o���w���ƹ��y��
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	//�I���e���]���¦�
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//���w�@�ӵ��U�������W�l
	window_class.lpszClassName = title;
	//��@�~�t�ε��U�@�ӷs������
	if(FALSE == RegisterClass(&window_class))
		return FALSE;

	DWORD window_style;

	if(g_GutCallBack.OnSize)//�p�G���B�z�����Y�񪺰T��,�~�e�\�ƹ����ܵ����j�p
		window_style = WS_OVERLAPPEDWINDOW;
	else
		window_style = WS_BORDER | WS_SYSMENU;

	//�p���������u�|�e�����I��,���n�ɼW�[�����j�p
	RECT window_rect;
	SetRect(&window_rect, x, y, x+width, y+height);
	AdjustWindowRect(&window_rect, window_style, FALSE);

	g_iWindowPosWidth = width;
	g_iWindowPosHeight = height;

	//�ǳƧ���,�}�Ҥ@�ӵ���
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

	//�O���U�ΨӥN�������ثe����{��������,�H��|�A�Ψ�
	g_hWnd = window_handle;
	g_hInstance = window_class.hInstance;
	//��ܵ���
	ShowWindow(window_handle, SW_SHOWNORMAL);
	SetActiveWindow(window_handle);

	return TRUE;
}

//##############################################################################
//##########################message proc##########################################
//##############################################################################

//�T���B�z
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

//�ˬdWindows�@�~�t�ζǨӪ��T��
bool GutProcessMessage(void)
{
	MSG	msg;
	//�ݬݦ��S������T��`
	if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
	{
		// ���T������, �N�⥦�e���T���B�z�禡.
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if ( msg.message==WM_QUIT )
		{
			//���������������T���|�Ǧ^false�ӵ����j��.
			return FALSE;
		}
	}
	return TRUE;
}
