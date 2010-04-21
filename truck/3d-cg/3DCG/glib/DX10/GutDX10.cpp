#include <windows.h>
#include <D3D10.h>
#include <D3DX10.h>

#include "..\Utility\Gut.h"
#include "..\Utility\GutFileUtility.h"
#include "..\Win32\GutWin32.h"
#include "GutDX10.h"


//#############################################################
//########################global unit#############################
//#############################################################

static ID3D10Device				*g_pd3dDevice = NULL;
static IDXGISwapChain			*g_pSwapChain = NULL;

static ID3D10Texture2D			*g_pDepthStencil = NULL;
static ID3D10RenderTargetView	*g_pRenderTargetView = NULL;
static ID3D10DepthStencilView	*g_pDepthStencilView = NULL;

static ID3D10Effect				*g_pDefaultEffect = NULL;
static ID3D10EffectTechnique	*g_pDefaultTechnique = NULL;
static ID3D10SamplerState		*g_pSamplerState = NULL;
static ID3D10RasterizerState	*g_pCull = NULL;

static D3D10_SHADER_MACRO *g_pHLSL_Macros = NULL;


void GutSetDX10DefaultRasterizerDesc(D3D10_RASTERIZER_DESC &desc)
{
	ZeroMemory(&desc, sizeof(desc));

	desc.FillMode = D3D10_FILL_SOLID;
	desc.CullMode = D3D10_CULL_BACK;
	desc.FrontCounterClockwise = FALSE;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = TRUE;
	desc.ScissorEnable = FALSE;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;
}

bool GutSetDefaultStatesDX10(void)
{
	// sampler state / texture filter
	{
		D3D10_SAMPLER_DESC desc;
		GutSetDX10DefaultSamplerDesc(desc);

		desc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;

		g_pd3dDevice->CreateSamplerState(&desc, &g_pSamplerState);

		for ( int i=0; i<4; i++ )
		{
			g_pd3dDevice->PSSetSamplers(i, 1, &g_pSamplerState);
		}
	}
	// rasterizer state
	{
		D3D10_RASTERIZER_DESC desc;
		GutSetDX10DefaultRasterizerDesc(desc);

		desc.CullMode = D3D10_CULL_BACK;
		desc.FrontCounterClockwise = TRUE;

		if ( D3D_OK != g_pd3dDevice->CreateRasterizerState(&desc, &g_pCull) )
			return FALSE;

		g_pd3dDevice->RSSetState(g_pCull);
	}

	return TRUE;
}

void GutSetDX10DefaultBlendDesc(D3D10_BLEND_DESC &desc)
{
	ZeroMemory(&desc, sizeof(desc));

	desc.AlphaToCoverageEnable = FALSE;
	
	desc.SrcBlend = D3D10_BLEND_ONE;
	desc.DestBlend = D3D10_BLEND_ZERO;
	desc.BlendOp = D3D10_BLEND_OP_ADD;
	
	desc.SrcBlendAlpha = D3D10_BLEND_ONE;
	desc.DestBlendAlpha = D3D10_BLEND_ZERO;
	desc.BlendOpAlpha = D3D10_BLEND_OP_ADD;

	for ( int i=0; i<8; i++ )
	{
		desc.RenderTargetWriteMask[i] = D3D10_COLOR_WRITE_ENABLE_ALL;
	}
}

void GutSetDX10DefaultDepthStencilDesc(D3D10_DEPTH_STENCIL_DESC &desc)
{
	ZeroMemory(&desc, sizeof(desc));

	desc.DepthEnable = TRUE;
	desc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D10_COMPARISON_LESS;

	desc.StencilEnable = FALSE;
	desc.StencilReadMask = 0xff;
	desc.StencilWriteMask = 0xff;

	desc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	desc.BackFace = desc.FrontFace;
}

void GutSetDX10DefaultSamplerDesc(D3D10_SAMPLER_DESC &desc)
{
	ZeroMemory(&desc, sizeof(desc));

	desc.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT;

	desc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;

	desc.ComparisonFunc = D3D10_COMPARISON_NEVER;

	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D10_FLOAT32_MAX;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 16;
}

ID3D10Device *GutGetGraphicsDeviceDX10(void)
{
	return g_pd3dDevice;
}

ID3D10RenderTargetView *GutGetDX10RenderTargetView(void)
{
	return g_pRenderTargetView;
}

ID3D10DepthStencilView *GutGetDX10DepthStencilView(void)
{
	return g_pDepthStencilView;
}

IDXGISwapChain *GutGetDX10SwapChain(void)
{
	return g_pSwapChain;
}

//#############################################################
//########################device init#############################
//#############################################################

bool GutInitGraphicsDeviceDX10(GutDeviceSpec *spec)
{
	HRESULT hr = S_OK;
	HWND hWnd = GutGetWindowHandleWin32();
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif
	//設定frontbuffer與backbuffer大小及格式
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count =1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	//開啟forntbuffer與buckbuffer
	hr = D3D10CreateDeviceAndSwapChain(
		NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		D3D10_SDK_VERSION,
		&sd,
		&g_pSwapChain,
		&g_pd3dDevice
		);
	if(FAILED(hr))
		return FALSE;
	//取出第一個disply buffer
	ID3D10Texture2D *pBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D10Texture2D),
		(LPVOID *)&pBuffer
		);
	if(FAILED(hr))
		return FALSE;
	//開啟一個合乎display buffer格式的RenderTargetView
	hr = g_pd3dDevice->CreateRenderTargetView(
		pBuffer,
		NULL,
		&g_pRenderTargetView
		);
	pBuffer->Release();
	if(FAILED(hr))
		return FALSE;

	//設定depth與stencil buffer的資料格式
	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags =0;
	//配置Depth與stencil buffer的空間
	hr = g_pd3dDevice->CreateTexture2D(
		&descDepth,
		NULL,
		&g_pDepthStencil
		);
	if(FAILED(hr))
		return FALSE;

	//設定Depth與stencil buffer資料格式
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	//開啟depth與stencil buffer的view
	hr = g_pd3dDevice->CreateDepthStencilView(
		g_pDepthStencil,
		&descDSV,
		&g_pDepthStencilView
		);
	if(FAILED(hr))
		return FALSE;

	g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	
	//viewport 用來指定3D繪圖的畫布範圍
	D3D10_VIEWPORT vp;

	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX =0;
	vp.TopLeftY =0;
	g_pd3dDevice->RSSetViewports(1, &vp);

	return TRUE;
}

//#############################################################
//########################shader tool#############################
//#############################################################

// 載入Vertex Shader
// file = HLSL shader file
// entry = vertex shader entry point
// profile = shader version
// pShaderCode = compiled shader binary code
ID3D10VertexShader* GutLoadVertexShaderDX10_HLSL(const char *filename, const char *entry, const char *profile, ID3D10Blob **pShaderCode)
{
    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;

#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10VertexShader *pShader = NULL;
    ID3D10Blob	*pBuf = NULL;
	ID3D10Blob	*pErrorMessage = NULL;

	char filename_fullpath[128];
	sprintf_s(filename_fullpath, "%s%s", GutGetShaderPath(), filename);

	if ( D3D_OK==D3DX10CompileFromFile(filename_fullpath, g_pHLSL_Macros, NULL, entry, profile, dwShaderFlags, NULL, NULL, &pBuf, &pErrorMessage, NULL) )
	{
		g_pd3dDevice->CreateVertexShader( (DWORD*) pBuf->GetBufferPointer(), pBuf->GetBufferSize(), &pShader);
	}
	else
	{
		if ( pErrorMessage )
		{
			printf("Failed to compile %s : %s\n", pErrorMessage->GetBufferPointer());
		}
	}

	SAFE_RELEASE(pErrorMessage);

	if ( pShaderCode )
	{
		*pShaderCode = pBuf;
	}
	else
	{
		SAFE_RELEASE(pBuf);
	}

	return pShader;
}
// 載入Pixel Shader
// file = HLSL shader file
// entry = pixel shader entry point
// profile = shader version
ID3D10PixelShader * GutLoadPixelShaderDX10_HLSL(const char *filename, const char *entry, const char *profile)
{
    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;

#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10PixelShader *pShader = NULL;
    ID3D10Blob	*pBuf = NULL;
	ID3D10Blob	*pErrorMessage = NULL;

	char filename_fullpath[128];
	sprintf_s(filename_fullpath, "%s%s", GutGetShaderPath(), filename);

	if ( D3D_OK==D3DX10CompileFromFile(filename_fullpath, g_pHLSL_Macros, NULL, entry, profile, dwShaderFlags, NULL, NULL, &pBuf, &pErrorMessage, NULL) )
	{
		g_pd3dDevice->CreatePixelShader( (DWORD*) pBuf->GetBufferPointer(), pBuf->GetBufferSize(), &pShader);
	}
	else
	{
		if ( pErrorMessage )
		{
			printf("Failed to compile %s : %s\n", pErrorMessage->GetBufferPointer());
		}
	}

	SAFE_RELEASE(pErrorMessage);
	SAFE_RELEASE(pBuf);
	
	return pShader;
}

ID3D10Effect *GutLoadFXShaderDX10(const char *filename)
{
    //DWORD dwShaderFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
    //DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob	*pErrorMessage = NULL;
	ID3D10Effect *pFXShader = NULL;

	char filename_fullpath[128];
	sprintf_s(filename_fullpath, "%s%s", GutGetShaderPath(), filename);

	if ( D3D_OK!=D3DX10CreateEffectFromFile( 
						filename_fullpath, g_pHLSL_Macros, NULL, 
						"fx_4_0", dwShaderFlags, 0, 
						g_pd3dDevice, NULL, NULL, 
						&pFXShader, &pErrorMessage, NULL) )
	{
		if ( pErrorMessage )
		{
			printf("Failed to compile %s : %s\n", pErrorMessage->GetBufferPointer());
		}
	}

	SAFE_RELEASE(pErrorMessage);
	
	return pFXShader;
}

//#############################################################
//####################resource release#############################
//#############################################################

bool GutResetGraphicsDeviceDX10(void)
{
	if ( g_pd3dDevice==NULL )
		return false;

	DWORD hr;

	// 取得視窗大小
	RECT rect;
	GetWindowRect( GutGetWindowHandleWin32(), &rect );

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 先把原本設定用來繪圖的framebuffer跟depthbuffer拿走
	g_pd3dDevice->OMSetRenderTargets(0, NULL, NULL);

	// 釋放framebuffer跟depthbuffer的資源
	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pDepthStencil);
	SAFE_RELEASE(g_pDepthStencilView);

	DXGI_MODE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = width;
	desc.Height = height;
	desc.RefreshRate.Numerator = 60;
	desc.RefreshRate.Denominator = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	// 改變swapchain的大小
	if ( D3D_OK!=g_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0) )
		return false;

	// 取出第一個display buffer
	ID3D10Texture2D *pBuffer = NULL;
	if ( D3D_OK!=g_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (LPVOID*)&pBuffer ) )
		return false;
	// 開啟一個合乎display buffer格式的RenderTargetView
	hr = g_pd3dDevice->CreateRenderTargetView( pBuffer, NULL, &g_pRenderTargetView );
	pBuffer->Release();
	if( FAILED( hr ) )
		return false;

	// 設定Depth/Stencil buffer的資料格式
	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	// 配置Depth/Stencil buffer的空間
	hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
	if( FAILED(hr) )
		return false;

	// 設定Depth/Stencil View的格式
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	// 開啟DepthStencil Buffer的View
	hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
	if( FAILED(hr) )
		return false;

	// 設定繪圖的結果會存放在什麼地方
	// RenderTargetView = RGBA Color Buffer resource
	// DepthStencilView = Depth/Stencil Buffer resource
	g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// Viewport用來指定3D繪圖的畫布範圍
	// 把Viewport設定成跟整個視窗一樣大
	D3D10_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	g_pd3dDevice->RSSetViewports( 1, &vp );

	return true;
}

bool GutReleaseGraphicsDeviceDX10(void)
{
	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pDepthStencilView);
	SAFE_RELEASE(g_pDepthStencil);
	SAFE_RELEASE(g_pSwapChain);
	SAFE_RELEASE(g_pd3dDevice);

	return true;
}