#ifndef _RENDER_OPENGL_H_
#define _RENDER_OPENGL_H_

bool InitResourceOpenGL(void);
bool ReleaseResourceOpenGL(void);
void RenderFrameOpenGL(void);

#include "..\glib\camera\gutcamera.h"

extern GutCamera g_camera;

#endif //_RENDER_OPENGL_H_
