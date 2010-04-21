// main.cpp : ﹚竡北莱ノ祘Α秈翴

#include "stdafx.h"

#include "render_data.h"
#include "dx10app.h"
#include "dx9_shader.h"
#include "opengl_shader.h"
#include "..\glib\Utility\Gut.h"



int _tmain(int argc, _TCHAR* argv[])
{
	// ず﹚ㄏノDirectX 9ㄓ酶瓜
	char *device = "opengl";
	void (*render)(void) = NULL;
	bool (*init_resource)(void) = NULL;
	bool (*release_resource)(void) = NULL;

	// 狦㏑把计﹚ノOpenGL, 碞эノOpenGL.
	if ( strcmp(device, "opengl")==0 )
	{
		init_resource = InitResourceOpenGL;
		release_resource = ReleaseResourceOpenGL;
		render = RenderFrameOpenGL;
	}
	// 狦㏑把计﹚ノDX10, 碞эノDX10.
	else if ( strcmp(device, "dx9")==0 )
	{
		init_resource = InitResourceDX9;
		release_resource = ReleaseResourceDX9;
		render = RenderFrameDX9;
	}
	else if ( strcmp(device, "dx10")==0 )
	{
		render = RenderFrameDX10;
		init_resource = InitResourceDX10;
		release_resource = ReleaseResourceDX10;
	}


	GutCreateWindow(100, 100, 512, 512, device);

	if(!GutInitGraphicsDevice(device))
	{
		printf("failed to initialize %s device \n",device);
		exit(0);
	}


	// 更shader
	if ( !init_resource() )
	{
		release_resource();
		printf("Failed to load resources\n");
		exit(0);
	}


	while(GutProcessMessage())
	{
		render();
	}
	release_resource();

	GutReleaseGraphicsDevice();
	return 0;
}