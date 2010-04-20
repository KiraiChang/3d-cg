#ifndef _GUT_DX9_H_
#define _GUT_DX9_H_

#include <d3d9.h>
//#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")

#include "GutDefs.h"

LPDIRECT3DDEVICE9 GutGetGraphicsDeviceDX9(void);


//---------------------device init---------------------------------
bool GutInitGraphicsDeviceDX9(GutDeviceSpec *pSpec = NULL);

//--------------------resource release----------------------------
bool GutReleaseGraphicsDeviceDX9(void);
bool GutResetGraphicsDeviceDX9(void);

#endif //_GUT_DX9_H_