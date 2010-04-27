// camera-rotation.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>

#include "..\glib\utility\Gut.h"
#include "..\glib\input\gutinput.h"
//#include "render_dx9.h"
//#include "render_dx10.h"
#include "render_opengl.h"
#include "render_data.h"
#include "..\glib\camera\gutcamera.h"
#include "..\glib\win32\GutTimer.h"

GutTimer g_Timer;

void GetUserInput(void)
{
	// `讀取滑鼠`
	GutMouseInfo mouse;
	GutReadMouse(&mouse);
	// `讀取鍵盤`
	char keyboard_state[256];
	GutReadKeyboard(keyboard_state);

	// `取得畫完前一個畫面到現在所經歷的時間`
	float time_diff = g_Timer.Stop();
	g_Timer.Restart();

	float moving_speed = 2.0f * time_diff;
	float rotation_speed = 1.0f * time_diff;

	//極座標系統
	static float theta = -MATH_PI * 0.5f;
	static float phi = 0.0f;

	//如果按下滑鼠左鍵, 就旋轉鏡頭.
	if ( mouse.button[0] ) 
	{
		theta += mouse.x * rotation_speed;
		phi -= mouse.y * rotation_speed;
	}

	//設定面向
	g_camera.SetFacing(phi, theta);

	//按下W或方向鍵向上
	if ( keyboard_state[GUTKEY_W] || keyboard_state[GUTKEY_UP] )
	{
		g_camera.Move(moving_speed);
	}
	//按下S或方向鍵向下
	if ( keyboard_state[GUTKEY_S] || keyboard_state[GUTKEY_DOWN] )
	{
		g_camera.Move(-moving_speed);
	}
	//按下W或方向鍵向上
	if ( keyboard_state[GUTKEY_Q] || keyboard_state[GUTKEY_UP] )
	{
		g_camera.Pitch(moving_speed);
	}
	//按下S或方向鍵向下
	if ( keyboard_state[GUTKEY_Z] || keyboard_state[GUTKEY_DOWN] )
	{
		g_camera.Pitch(-moving_speed);
	}

	//按下A或方向鍵向左
	if ( keyboard_state[GUTKEY_A] || keyboard_state[GUTKEY_LEFT] )
	{
		g_camera.Yaw(-moving_speed);
	}
	//按下D或方向鍵向右
	if ( keyboard_state[GUTKEY_D] || keyboard_state[GUTKEY_RIGHT] )
	{
		g_camera.Yaw(moving_speed);
	}

	/*
	float ry = 0.0f;

	// `如果按下滑鼠左鍵, 就旋轉鏡頭.`
	if ( mouse.button[0] ) 
		ry = mouse.x * rotation_speed;
	// `按下A或是方向鍵<-, 就左旋.`
	if ( keyboard_state[GUTKEY_A] || keyboard_state[GUTKEY_LEFT] )
		ry = -rotation_speed;
	// `按下D或是方向鍵->, 就右旋.`
	if ( keyboard_state[GUTKEY_D] || keyboard_state[GUTKEY_RIGHT] )
		ry = rotation_speed;

	//鏡頭左右旋轉
	g_camera.Rotation(ry);

	// `按下W或方向鍵向上`
	if ( keyboard_state[GUTKEY_W] || keyboard_state[GUTKEY_UP] )
	{
		//鏡頭向前移動
		//g_camera.Move(moving_speed);
	}

	// `按下S或方向鍵向下`
	if ( keyboard_state[GUTKEY_S] || keyboard_state[GUTKEY_DOWN] )
	{
		//鏡頭向後移動
		//g_camera.Move(-moving_speed);
	}
	*/

	// 計算出鏡頭對準的點, 產生鏡頭轉換矩陣時會用到.
	g_camera.SetLookUp();
}

int _tmain(int argc, _TCHAR* argv[])
{
		// 內定使用DirectX 9來繪圖
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
	// 在(100,100)的位置開啟一個大小為(512x512)的視窗
	GutCreateWindow(100, 100, 512, 512, device);

	// 做OpenGL或DirectX初始化
	if ( !GutInitGraphicsDevice(device) )
	{
		printf("Failed to initialize %s device\n", device);
		exit(0);
	}

	// 載入shader
	if ( !init_resource() )
	{
		release_resource();
		printf("Failed to load resources\n");
		exit(0);
	}

	GutInputInit();
	g_object_matrix.Identity();

	// 主迴圈
	while( GutProcessMessage() )
	{
		GetUserInput();
		render();
	}
	
	// 卸載shader
	release_resource();

	// 關閉OpenGL/DirectX繪圖裝置
	GutReleaseGraphicsDevice();
	return 0;
}

