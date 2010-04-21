#include "stdafx.h"
#include "opengl_shader.h"
#include "render_data.h"
#include <windows.h>
// OpenGL extensions header
#include "..\Glib\OpenGL\glew.h"
#include "..\Glib\OpenGL\wglew.h"
// Standard OpenGL header
#include <GL/gl.h>
#include "..\glib\Utility\Gut.h"

//#############################################################
//########################no shader##############################
//#############################################################

/*
void RenderFrameOpenGL(void)
{
	//清除畫面
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	//計算一個使用平行投影的矩陣
	Matrix4x4 perspective_matrix = GutMatrixPerspectiveRH_OpenGL(90.0f, 1.0f, 1.0f, 100.0f);
	//把這兩矩陣相乘
	Matrix4x4 view_perspective_matrix = view_matrix * perspective_matrix;
	//把空間中的座標點轉換到螢幕座標系統
	Vector4 vertices[16];
	for(int i =0; i<16 ;i++)
	{
		vertices[i] = g_vertices[i] * view_perspective_matrix;
		vertices[i] /= vertices[i].GetW();
	}
	
	//劃出金字塔的8條邊線
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(4, GL_FLOAT, sizeof(Vector4), vertices);
	glDrawArrays(GL_LINES, 0, 16);
	//把背景backbuffer的畫面呈現出來
	GutSwapBuffersOpenGL();
}
*/

//#############################################################
//######################have shader##############################
//#############################################################

static GLuint g_VertexProgramID = NULL;
static GLuint g_FragmentProgramID = NULL;

bool InitResourceOpenGL(void)
{
	g_VertexProgramID = GutLoadVertexProgramOpenGL_ASM("vertex_transform.glvp");
	if(NULL == g_VertexProgramID)
		return FALSE;

	g_FragmentProgramID = GutLoadFragmentProgramOpenGL_ASM("vertex_transform.glpp");
	if(NULL == g_FragmentProgramID)
		return FALSE;
	
	//鏡頭轉換矩陣
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	//計算一個使用平行投影的矩陣
	Matrix4x4 perspective_matrix = GutMatrixPerspectiveRH_OpenGL(90.0f, 1.0f, 1.0f, 100.0f);
	//把這兩矩陣相乘
	Matrix4x4 view_perspective_matrix = view_matrix * perspective_matrix;

	//設定鏡頭轉換矩陣
	glProgramLocalParameters4fvEXT(GL_VERTEX_PROGRAM_ARB, 0, 4, (float *)&view_perspective_matrix);

	return TRUE;
}

bool ReleaseResourceOpenGL(void)
{
	if(g_VertexProgramID)
	{
		GutReleaseVertexProgramOpenGL(g_VertexProgramID);
		g_VertexProgramID = NULL;
	}

	if(g_FragmentProgramID)
	{
		GutReleaseFragmentProgramOpenGL(g_FragmentProgramID);
		g_FragmentProgramID = NULL;
	}
	return TRUE;
}

void RenderFrameOpenGL(void)
{
	//清除畫面
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//指定要使用shader可程式化流程
	glEnable(GL_VERTEX_PROGRAM_ARB);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	//設定用哪個vertex/fragment program
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, g_VertexProgramID);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, g_FragmentProgramID);

	//劃出金字塔的8條邊線
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(4, GL_FLOAT, sizeof(Vector4), g_vertices);
	glDrawArrays(GL_LINES, 0, 16);
	//把背景backbuffer的畫面呈現出來
	GutSwapBuffersOpenGL();
}