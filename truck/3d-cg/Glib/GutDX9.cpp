#include <windows.h>
#include <D3D9.h>
//#include <D3DX9.h>
#include <stdio.h>

#include "GutWin32.h"
#include "GutDX9.h"
#include "Gut.h"

//#############################################################
//########################global unit#############################
//#############################################################

static LPDIRECT3D9 g_pD3D = NULL;
static LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
static D3DPRESENT_PARAMETERS g_pD3DPresent;

LPDIRECT3DDEVICE9 GutGetGraphicsDeviceDX9(void)
{
	// �����Ǧ^�w�g��l�Ʀn��Direct3D 9�˸m
	return g_pD3DDevice;
}

//#############################################################
//########################device init#############################
//#############################################################

bool GutInitGraphicsDeviceDX9(GutDeviceSpec *pSpec)
{
	int multisamples = 0;
	if(pSpec)
		multisamples = pSpec->m_iMultiSamples;
	//�}�ҵ�����N�w�����o�Ӫ���ΨӥN�����������
	HWND hWnd = GutGetWindowHandleWin32();

	//���oD3D9����
	if( NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return FALSE;

	ZeroMemory(&g_pD3DPresent, sizeof(g_pD3DPresent));
	g_pD3DPresent.Windowed = TRUE;
	g_pD3DPresent.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_pD3DPresent.BackBufferFormat = D3DFMT_UNKNOWN;
	g_pD3DPresent.BackBufferCount = 1;//���Ѥ@��backbuffer
	g_pD3DPresent.EnableAutoDepthStencil = TRUE;
	g_pD3DPresent.AutoDepthStencilFormat = D3DFMT_D24S8;
	g_pD3DPresent.MultiSampleType = (D3DMULTISAMPLE_TYPE) multisamples;

	bool device_initialized = FALSE;

	const int DEVICE_TYPES = 4;
	struct sDeviceType
	{
		D3DDEVTYPE type;
		DWORD behavior;
	};

	sDeviceType device_type[DEVICE_TYPES] = 
	{
		{D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING},
		{D3DDEVTYPE_HAL, D3DCREATE_MIXED_VERTEXPROCESSING},
		{D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING},
		{D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING}
	};

	for(int type = 0; type < DEVICE_TYPES; type++)
	{
		//�յۥh�}�Ҥ@��Direct3d9�˸m
		if( D3D_OK == g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			device_type[type].type,
			hWnd,
			device_type[type].behavior,
			&g_pD3DPresent,
			&g_pD3DDevice)
			)
		{
			device_initialized = TRUE;
			break;
		}
	}
	return device_initialized;
}

//#############################################################
//####################resource release#############################
//#############################################################


bool GutReleaseGraphicsDeviceDX9(void)
{
	if ( g_pD3DDevice )
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if ( g_pD3D )
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	
	return TRUE;
}

bool GutResetGraphicsDeviceDX9(void)
{
	RECT rect;
	GetWindowRect( GutGetWindowHandleWin32(), &rect);
	
	g_pD3DPresent.BackBufferWidth = 0;
	g_pD3DPresent.BackBufferHeight = 0;
	g_pD3DPresent.BackBufferCount = 1; // `���Ѥ@��backbuffer`

	if ( D3D_OK==g_pD3DDevice->Reset(&g_pD3DPresent) )
		return TRUE;
	else
		return FALSE;
}
