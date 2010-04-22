// object-rotation.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>

#include "..\glib\utility\Gut.h"
//#include "render_dx9.h"
//#include "render_dx10.h"
#include "render_opengl.h"


int _tmain(int argc, _TCHAR* argv[])
{
		// ���w�ϥ�DirectX 9��ø��
	char *device = "opengl";
	void (*render)(void) = RenderFrameOpenGL;
	bool (*init_resource)(void) = InitResourceOpenGL;
	bool (*release_resource)(void) = ReleaseResourceOpenGL;
	/*
	printf("Press\n(1) for Direct3D9\n(2) for OpenGL\n(3) for Direct3D10\n");

	int c = getche();
	switch(c)
	{
	default:
	case '1':
		render = RenderFrameDX9;
		init_resource = InitResourceDX9;
		release_resource = ReleaseResourceDX9;
		break;
	case '2':
		device = "opengl";
		init_resource = InitResourceOpenGL;
		release_resource = ReleaseResourceOpenGL;
		render = RenderFrameOpenGL;
		break;
	case '3':
		device = "dx10";
		init_resource = InitResourceDX10;
		release_resource = ReleaseResourceDX10;
		render = RenderFrameDX10;
		break;
	}
	*/
	// �b(100,100)����m�}�Ҥ@�Ӥj�p��(512x512)������
	GutCreateWindow(100, 100, 512, 512, device);

	// ��OpenGL��DirectX��l��
	if ( !GutInitGraphicsDevice(device) )
	{
		printf("Failed to initialize %s device\n", device);
		exit(0);
	}

	// ���Jshader
	if ( !init_resource() )
	{
		release_resource();
		printf("Failed to load resources\n");
		exit(0);
	}

	// �D�j��
	while( GutProcessMessage() )
	{
		render();
	}
	
	// ����shader
	release_resource();

	// ����OpenGL/DirectXø�ϸ˸m
	GutReleaseGraphicsDevice();
	return 0;
}

