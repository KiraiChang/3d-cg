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
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(90.0f, 1.0f, 1.0f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((float *)&projection_matrix);
	glMatrixMode(GL_MODELVIEW);	

	glEnable(GL_CULL_FACE);

	return TRUE;
}

void RenderFrameOpenGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//設定用陣列方式讀取記憶體資料
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);

	Vector4 border(-15.0f, 0.0f, -15.0f);
	Vector4 grid_position = border;

	const int GRIDS_X = 30;
	const int GRIDS_Z = 30;

	for (int x = 0; x < GRIDS_X; x++)
	{
		int grid_x = x & 0x07;
		grid_position[2] = border[2];

		for(int z = 0; z < GRIDS_Z; z++)
		{
			int grid_z = z &0x07;
			char c = g_map[grid_x][grid_z];

			//設定轉換矩陣
			Matrix4x4 object_matrix; object_matrix.Identity();

			if( c == 0)
			{
				object_matrix = view_matrix;
				object_matrix.Translate(grid_position);
				glLoadMatrixf((float *)&object_matrix);

				//設定GPU要去哪讀取頂點座標資料
				glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), &g_road_vertices[0].m_Position);
				//設定GPU要去哪讀取頂點顏色資料
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), &g_road_vertices[0].m_RGBA);

				//劃出地板
				glDrawElements(
					GL_TRIANGLE_STRIP,
					4,
					GL_UNSIGNED_SHORT,
					g_road_trianglestrip_indices
					);
			}
			else
			{
				object_matrix.Scale_Replace(1.0f, (float)c, 1.0f);
				object_matrix[3] = grid_position;
				Matrix4x4 world_view_matrix  = object_matrix * view_matrix;
				glLoadMatrixf((float *)&world_view_matrix);

				//設定GPU要去哪讀取頂點座標資料
				glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), &g_pyramid_vertices[0].m_Position);
				//設定GPU要去哪讀取頂點顏色資料
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), &g_pyramid_vertices[0].m_RGBA);

				//劃出金字塔
				glDrawElements(
					GL_TRIANGLE_FAN,
					6,
					GL_UNSIGNED_SHORT,
					g_pyramid_trianglefan_indices
					);
			}
			grid_position[2] += 1.0f;
		}
		grid_position[0] += 1.0f;
	}
	//把背景backbuffer的畫面呈現出來
	GutSwapBuffersOpenGL();
}

bool ReleaseResourceOpenGL(void)
{
	// 沒事做
	return TRUE;
}
