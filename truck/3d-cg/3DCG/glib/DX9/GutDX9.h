#ifndef _GUT_DX9_H_
#define _GUT_DX9_H_

#include <D3D9.h>
#include <D3DX9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "..\Utility\GutDefs.h"

LPDIRECT3DDEVICE9 GutGetGraphicsDeviceDX9(void);

//--------------------shader tool----------------------------------
LPDIRECT3DVERTEXSHADER9 GutLoadVertexShaderDX9_HLSL(const char *filename, const char *entry, const char *profile);
LPDIRECT3DPIXELSHADER9  GutLoadPixelShaderDX9_HLSL(const char *filename, const char *entry, const char *profile);

LPDIRECT3DVERTEXSHADER9 GutLoadVertexShaderDX9_ASM(const char *filename);
LPDIRECT3DPIXELSHADER9  GutLoadPixelShaderDX9_ASM(const char *filename);

LPD3DXEFFECT GutLoadFXShaderDX9(const char *filename);
//---------------------device init---------------------------------
bool GutInitGraphicsDeviceDX9(GutDeviceSpec *pSpec = NULL);

//--------------------resource release----------------------------
bool GutReleaseGraphicsDeviceDX9(void);
bool GutResetGraphicsDeviceDX9(void);

#endif //_GUT_DX9_H_