// main.cpp : �w�q�D���x���ε{�����i�J�I�C

#include "stdafx.h"

#include "render_data.h"
#include "dx10app.h"
#include "dx9_shader.h"
#include "opengl_shader.h"
#include "..\glib\Utility\Gut.h"



int _tmain(int argc, _TCHAR* argv[])
{
	// ���w�ϥ�DirectX 9��ø��
	char *device = "opengl";
	void (*render)(void) = NULL;
	bool (*init_resource)(void) = NULL;
	bool (*release_resource)(void) = NULL;

	// �p�G�R�O�C�Ѽƫ��w��OpenGL, �N���OpenGL.
	if ( strcmp(device, "opengl")==0 )
	{
		init_resource = InitResourceOpenGL;
		release_resource = ReleaseResourceOpenGL;
		render = RenderFrameOpenGL;
	}
	// �p�G�R�O�C�Ѽƫ��w��DX10, �N���DX10.
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


	// ���Jshader
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