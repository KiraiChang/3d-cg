#ifndef _GUT_DX10_H_
#define _GUT_DX10_H_

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")

#include "..\Utility\GutDefs.h"

void GutSetDX10DefaultRasterizerDesc(D3D10_RASTERIZER_DESC &desc);
void GutSetDX10DefaultBlendDesc(D3D10_BLEND_DESC &desc);
void GutSetDX10DefaultDepthStencilDesc(D3D10_DEPTH_STENCIL_DESC &desc);
void GutSetDX10DefaultSamplerDesc(D3D10_SAMPLER_DESC &desc);
ID3D10Device *GutGetGraphicsDeviceDX10(void);
ID3D10RenderTargetView *GutGetDX10RenderTargetView(void);
ID3D10DepthStencilView *GutGetDX10DepthStencilView(void);
IDXGISwapChain *GutGetDX10SwapChain(void);


//--------------device init--------------------------------

bool GutInitGraphicsDeviceDX10(GutDeviceSpec *spec=NULL);

//---------------shader tool------------------------------

ID3D10VertexShader* GutLoadVertexShaderDX10_HLSL(const char *filename, const char *entry, const char *profile, ID3D10Blob **pCode=NULL);
ID3D10PixelShader * GutLoadPixelShaderDX10_HLSL(const char *filename, const char *entry, const char *profile);
ID3D10Effect *GutLoadFXShaderDX10(const char *filename);

//--------------resource release--------------------------

bool GutReleaseGraphicsDeviceDX10(void);
bool GutResetGraphicsDeviceDX10(void);

#endif _GUT_DX10_H_