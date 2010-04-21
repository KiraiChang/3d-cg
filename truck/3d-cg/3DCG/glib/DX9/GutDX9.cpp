#include <windows.h>
#include <stdio.h>

#include "..\Win32\GutWin32.h"
#include "GutDX9.h"
#include "..\Utility\Gut.h"
#include "..\Utility\GutFileUtility.h"


//#############################################################
//########################global unit#############################
//#############################################################

static LPDIRECT3D9 g_pD3D = NULL;
static LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
static D3DPRESENT_PARAMETERS g_pD3DPresent;

LPDIRECT3DDEVICE9 GutGetGraphicsDeviceDX9(void)
{
	// 直接傳回已經初始化好的Direct3D 9裝置
	return g_pD3DDevice;
}

//#############################################################
//########################shader tool#############################
//#############################################################

// 載入Vertex Shader
// file = HLSL shader file
// entry = vertx shader entry point
// profile = shader version
LPDIRECT3DVERTEXSHADER9 GutLoadVertexShaderDX9_HLSL(const char *filename, const char *entry, const char *profile)
{
	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pErrorMsg = NULL;
	DWORD flags = 0;
#ifdef _DEBUG
	flags |= D3DXSHADER_DEBUG;
#endif

	char filename_fullpath[256];
	sprintf_s(filename_fullpath, "%s%s", GutGetShaderPath(), filename);

	HRESULT result = D3DXCompileShaderFromFile(
		filename_fullpath,
		NULL,
		NULL,
		entry,
		profile,
		flags,
		&pShader,
		&pErrorMsg,
		NULL
		);
	
	if ( result != S_OK )
	{
		if ( pErrorMsg )
		{
			printf("%s\n", pErrorMsg->GetBufferPointer() );
			pErrorMsg->Release();
		}
		return NULL;
	}

	LPDIRECT3DVERTEXSHADER9 pVertexShader = NULL;
	result = g_pD3DDevice->CreateVertexShader( (DWORD*)pShader->GetBufferPointer(), &pVertexShader);
	if ( result!=S_OK )
	{
		printf("Failed to load vertex shader %s\n", filename);	
	}
	
	pShader->Release();

	return pVertexShader;
}

// 載入Pixel Shader
// file = HLSL shader file
// entry = pixel shader entry point
// profile = shader version
LPDIRECT3DPIXELSHADER9  GutLoadPixelShaderDX9_HLSL(const char *filename, const char *entry, const char *profile)
{
	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pErrorMsg = NULL;
	DWORD flags = 0;
#ifdef _DEBUG
	flags |= D3DXSHADER_DEBUG;
#endif

	char filename_fullpath[256];
	sprintf_s(filename_fullpath, "%s%s", GutGetShaderPath(), filename);

	HRESULT result = D3DXCompileShaderFromFile(
		filename_fullpath,
		NULL,
		NULL,
		entry,
		profile,
		flags,
		&pShader,
		&pErrorMsg,
		NULL
		);
	
	if ( result != S_OK )
	{
		if ( pErrorMsg )
		{
			printf("%s\n", pErrorMsg->GetBufferPointer() );
			pErrorMsg->Release();
		}
		return NULL;
	}

	LPDIRECT3DPIXELSHADER9 pPixelShader = NULL;
	result = g_pD3DDevice->CreatePixelShader( (DWORD*)pShader->GetBufferPointer(), &pPixelShader);
	if ( result!=S_OK )
	{
		printf("Failed to load pixel shader %s\n", filename);	
	}
	
	pShader->Release();

	return pPixelShader;
}

LPDIRECT3DVERTEXSHADER9 GutLoadVertexShaderDX9_ASM(const char *filename)
{
	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pErrorMsg = NULL;
	DWORD flags = 0;
#ifdef _DEBUG
	flags |= D3DXSHADER_DEBUG;
#endif

	char filename_fullpath[256];
	sprintf_s(filename_fullpath, "%s%s", GutGetShaderPath(), filename);

	HRESULT result = D3DXAssembleShaderFromFile(
		filename_fullpath,
		NULL,
		NULL,
		flags,
		&pShader,
		&pErrorMsg
	);
	
	if ( result != S_OK )
	{
		if ( pErrorMsg )
		{
			printf("%s\n", pErrorMsg->GetBufferPointer() );
			pErrorMsg->Release();
		}
	}

	LPDIRECT3DVERTEXSHADER9 pVertexShader = NULL;
	result = g_pD3DDevice->CreateVertexShader( (DWORD*)pShader->GetBufferPointer(), &pVertexShader);
	if ( result!=S_OK )
	{
		printf("Failed to load vertex shader %s\n", filename);	
	}
	
	pShader->Release();

	return pVertexShader;
}

LPDIRECT3DPIXELSHADER9  GutLoadPixelShaderDX9_ASM(const char *filename)
{
	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pErrorMsg = NULL;
	DWORD flags = 0;
#ifdef _DEBUG
	flags |= D3DXSHADER_DEBUG;
#endif

	char filename_fullpath[256];
	sprintf_s(filename_fullpath, "%s%s", GutGetShaderPath(), filename);

	HRESULT result = D3DXAssembleShaderFromFile(
		filename_fullpath,
		NULL,
		NULL,
		flags,
		&pShader,
		&pErrorMsg
	);
	
	if ( result != S_OK )
	{
		if ( pErrorMsg )
		{
			printf("%s\n", pErrorMsg->GetBufferPointer() );
			pErrorMsg->Release();
		}
	}

	LPDIRECT3DPIXELSHADER9 pPixelShader = NULL;
	result = g_pD3DDevice->CreatePixelShader( (DWORD*)pShader->GetBufferPointer(), &pPixelShader);
	if ( result!=S_OK )
	{
		printf("Failed to load pixel shader %s\n", filename);	
	}
	
	pShader->Release();

	return pPixelShader;
}

LPD3DXEFFECT GutLoadFXShaderDX9(const char *filename)
{
	LPD3DXEFFECT pEffect;
	LPD3DXBUFFER pErrorMsg = NULL;
	DWORD flags = 0;
#ifdef _DEBUG
	flags |= D3DXSHADER_DEBUG;
#endif

	char filename_fullpath[256];
	sprintf_s(filename_fullpath, "%s%s", GutGetShaderPath(), filename);

	if ( D3D_OK!=D3DXCreateEffectFromFile(g_pD3DDevice, filename_fullpath, NULL, NULL, flags, NULL, &pEffect, &pErrorMsg) )
	{
		if ( pErrorMsg )
		{
			printf("%s\n", pErrorMsg->GetBufferPointer() );
		}
	}

	SAFE_RELEASE(pErrorMsg);

	return pEffect;
}

//#############################################################
//########################device init#############################
//#############################################################

bool GutInitGraphicsDeviceDX9(GutDeviceSpec *pSpec)
{
	int multisamples = 0;
	if(pSpec)
		multisamples = pSpec->m_iMultiSamples;
	//開啟視窗後就已取的這個物件用來代表視窗的指標
	HWND hWnd = GutGetWindowHandleWin32();

	//取得D3D9物件
	if( NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return FALSE;

	ZeroMemory(&g_pD3DPresent, sizeof(g_pD3DPresent));
	g_pD3DPresent.Windowed = TRUE;
	g_pD3DPresent.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_pD3DPresent.BackBufferFormat = D3DFMT_UNKNOWN;
	g_pD3DPresent.BackBufferCount = 1;//提供一塊backbuffer
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
		//試著去開啟一個Direct3d9裝置
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
	g_pD3DPresent.BackBufferCount = 1; // `提供一塊backbuffer`

	if ( D3D_OK==g_pD3DDevice->Reset(&g_pD3DPresent) )
		return TRUE;
	else
		return FALSE;
}
