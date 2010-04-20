#ifndef _GUT_DEF_H_
#define _GUT_DEF_H_

#define SAFE_RELEASE(x) if (x) { x->Release(); x=NULL; }
#define RELEASE_ARRAY(x) if ( x ) { delete [] x; x=NULL; }

#include <stdio.h>
#include ".\math\Vector4.h"

//--------------device init struct----------------

enum GutEnum
{
	GUT_UNKNOWN = 1,// device
	GUT_OPENGL,
	GUT_DX9,
	GUT_DX10,
};

struct GutCallBack
{
	void (*OnSize)(int x, int y);
	void (*OnPaint)(void);
	void (*OnIdle)(void);
	void (*OnClose)(void);

	GutCallBack(void);
};

struct GutDeviceSpec
{
	int m_iX, m_iY; // position
	int m_iWidth, m_iHeight; // size
	int m_iMultiSamples;
	bool m_bFullScreen; // fullscreen or windowed
	char *m_szDevice;

	GutDeviceSpec(void)
	{
		m_iX = m_iY = 256;
		m_iWidth = m_iHeight = 512;
		m_iMultiSamples = 0;
		m_bFullScreen = false;
		m_szDevice = NULL;
	}
};

//----------------------------------------



#endif //_GUT_DEF_H_