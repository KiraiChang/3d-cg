#include "Gut.h"
#include "GutWin32.h"

#include <memory.h>

GutCallBack g_GutCallBack;

GutCallBack::GutCallBack(void)
{
	memset(this, 0, sizeof(GutCallBack));
}
