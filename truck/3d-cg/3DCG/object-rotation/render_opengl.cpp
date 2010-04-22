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
	//�]�w�nGPU���Ū�����I���
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), g_vertices[0].m_Position);
	//�]�w�nGPU�n�h��Ū���C�⪺���
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), g_vertices[0].m_RGBA);

	//�ܧ�GL_MODEVIEW�x�}
	glMatrixMode(GL_MODELVIEW);
	//���ਤ��
	static float angle = 0.0f;
	angle += 0.001f;
	Matrix4x4 world_view_matrix = g_view_matrix;
	world_view_matrix.RotateZ(angle);
	//�]�w�ഫ�x�}
	glLoadMatrixf((float *)&world_view_matrix);

	//���X���r�𪺤K���u
	glDrawElements(
		GL_TRIANGLES,
		18,
		GL_UNSIGNED_SHORT,
		g_indices
		);
	//��I��backbuffer���e���e�{�X��
	GutSwapBuffersOpenGL();
}

bool ReleaseResourceOpenGL(void)
{
	// �S�ư�
	return TRUE;
}
