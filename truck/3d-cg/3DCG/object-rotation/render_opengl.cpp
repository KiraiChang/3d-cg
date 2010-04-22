#include "stdafx.h"
#include "render_opengl.h"

#include <windows.h>
// Standard OpenGL header
#include <GL/gl.h>

#include "..\glib\utility\Gut.h"
#include "render_data.h"

static Matrix4x4 g_view_matrix;

bool InitResourceOpenGL(void)
{
	g_view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(60.0f, 1.0f, 1.0f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((float *)&projection_matrix);
	return TRUE;
}

void RenderFrameOpenGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//設定好GPU到哪讀取頂點資料
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), g_vertices[0].m_Position);
	//設定好GPU要去哪讀取顏色的資料
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), g_vertices[0].m_RGBA);

	//變更GL_MODEVIEW矩陣
	glMatrixMode(GL_MODELVIEW);
	//旋轉角度
	static float angle = 0.0f;
	angle += 0.001f;
	Matrix4x4 world_view_matrix = g_view_matrix;
	world_view_matrix.RotateZ(angle);
	//設定轉換矩陣
	glLoadMatrixf((float *)&world_view_matrix);

	//劃出金字塔的八條線
	glDrawElements(
		GL_TRIANGLES,
		18,
		GL_UNSIGNED_SHORT,
		g_indices
		);
	//把背景backbuffer的畫面呈現出來
	GutSwapBuffersOpenGL();
}

bool ReleaseResourceOpenGL(void)
{
	// 沒事做
	return TRUE;
}
