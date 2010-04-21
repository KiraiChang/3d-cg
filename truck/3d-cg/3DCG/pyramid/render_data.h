#ifndef RENDER_DATA_H_
#define RENDER_DATA_H_

#include "..\glib\math\vector4.h"

static float vertex[] = {0.0f, 0.0f, 0.0f}; 

static Vector4 g_vertices[] = 
{
	Vector4(-1.0f, 1.0f,-1.0f),
	Vector4(-1.0f,-1.0f,-1.0f),

	Vector4(-1.0f,-1.0f,-1.0f),
	Vector4( 1.0f,-1.0f,-1.0f),

	Vector4( 1.0f,-1.0f,-1.0f),
	Vector4( 1.0f, 1.0f,-1.0f),

	Vector4( 1.0f, 1.0f,-1.0f),
	Vector4(-1.0f, 1.0f,-1.0f),

	Vector4( 0.0f, 0.0f, 1.0f),
	Vector4(-1.0f, 1.0f,-1.0f),

	Vector4( 0.0f, 0.0f, 1.0f),
	Vector4(-1.0f,-1.0f,-1.0f),

	Vector4( 0.0f, 0.0f, 1.0f),
	Vector4( 1.0f,-1.0f,-1.0f),

	Vector4( 0.0f, 0.0f, 1.0f),
	Vector4( 1.0f, 1.0f,-1.0f)
};
//���Y��m
static Vector4 g_eye(0.0f, 3.0f, 3.0f);
//���Y��Ǫ��I
static Vector4 g_lookat(0.0f, 0.0f, 0.0f);
//���Y�W��
static Vector4 g_up(0.0f, -1.0f, 1.0f);

#endif //RENDER_DATA_H_