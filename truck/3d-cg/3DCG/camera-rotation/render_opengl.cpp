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
	//�]�w�ΰ}�C�覡Ū���O������
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

			//�]�w�ഫ�x�}
			Matrix4x4 object_matrix; object_matrix.Identity();

			if( c == 0)
			{
				object_matrix = view_matrix;
				object_matrix.Translate(grid_position);
				glLoadMatrixf((float *)&object_matrix);

				//�]�wGPU�n�h��Ū�����I�y�и��
				glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), &g_road_vertices[0].m_Position);
				//�]�wGPU�n�h��Ū�����I�C����
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), &g_road_vertices[0].m_RGBA);

				//���X�a�O
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

				//�]�wGPU�n�h��Ū�����I�y�и��
				glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), &g_pyramid_vertices[0].m_Position);
				//�]�wGPU�n�h��Ū�����I�C����
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), &g_pyramid_vertices[0].m_RGBA);

				//���X���r��
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
	//��I��backbuffer���e���e�{�X��
	GutSwapBuffersOpenGL();
}

bool ReleaseResourceOpenGL(void)
{
	// �S�ư�
	return TRUE;
}
