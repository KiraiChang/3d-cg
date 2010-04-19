#include "Gut.h"
#include "GutWin32.h"

#include <memory.h>

GutCallBack g_GutCallBack;
static char g_szDeviceName[8];
GutEnum g_eDeviceType = GUT_UNKNOWN;

//#############################################################
//########################device init#############################
//#############################################################

bool GutInitGraphicsDevice(const char *device)
{
	strcpy_s(g_szDeviceName, device);
	g_eDeviceType = GutGetGraphicsDeviceType(device);

	switch(g_eDeviceType)
	{
	case GUT_OPENGL:
		return GutInitGraphicsDeviceOpenGL();
		break;
	//case GUT_DX9:
	//	return GutInitGraphicsDeviceDX9();
	//	break;
	//case GUT_DX10:
	//	return GutInitGraphicsDeviceDX10();
	//	break;
	}

	g_eDeviceType = GUT_UNKNOWN;
	g_szDeviceName[0] = '\0';

	return false;
}

GutEnum GutGetGraphicsDeviceType(const char *device)
{
	if ( device )
	{
		if ( !_stricmp(device, "opengl") )
		{
			return GUT_OPENGL;
		}
		else if ( !_stricmp(device, "dx9") )
		{
			return GUT_DX9;
		}
		else if ( !_stricmp(device, "dx10") )
		{
			return GUT_DX10;
		}
		else
		{
			return GUT_UNKNOWN;
		}
	}
	else
	{
		return g_eDeviceType;
	}
}

//#############################################################
//######################device operation##########################
//#############################################################


Matrix4x4 GutMatrixLookAtRH(Vector4 &eye, Vector4 &lookat, Vector4 &up)
{
	//找出鏡頭後方,右方,上方3個軸向.右手座標系統把-z視為前方
	Vector4 up_normalized = VectorNormalize(up);
	Vector4 zaxis = (eye - lookat); zaxis.Normalize();//鏡頭面向的反向即是後方
	Vector4 xaxis = Vector3CrossProduct(up_normalized, zaxis);//鏡頭右方
	Vector4 yaxis = Vector3CrossProduct(zaxis, xaxis);//鏡頭上方

	Matrix4x4 matrix;
	matrix.Identity();
	//快速反矩陣計算
	matrix.SetColumn(0, xaxis);
	matrix.SetColumn(1, yaxis);
	matrix.SetColumn(2, zaxis);
	matrix[3][0] = -Vector3Dot(xaxis, eye)[0];
	matrix[3][1] = -Vector3Dot(yaxis, eye)[0];
	matrix[3][2] = -Vector3Dot(zaxis, eye)[0];

	return matrix;
}

Matrix4x4 GutMatrixOrthoRH_OpenGL(float w, float h, float z_near, float z_far)
{
	Matrix4x4 matrix;
	matrix.Identity();

	matrix[0][0] = 2.0f/w;
	matrix[1][1] = 2.0f/h;
	matrix[2][2] = 2.0f/(z_near - z_far);
	matrix[3][2] = (z_far + z_near)/(z_near - z_far);
	matrix[3][3] = 1.0f;

	return matrix;
}

//#############################################################
//######################message proc#############################
//#############################################################


GutCallBack::GutCallBack(void)
{
	memset(this, 0, sizeof(GutCallBack));
}

//#############################################################
//####################resource release#############################
//#############################################################


bool GutReleaseGraphicsDevice(void)
{
	if ( !_stricmp(g_szDeviceName, "opengl") )
	{
		return GutReleaseGraphicsDeviceOpenGL();
	}
	//else if ( !_stricmp(g_szDeviceName, "dx9") )
	//{
	//	return GutReleaseGraphicsDeviceDX9();
	//}
	//else if ( !_stricmp(g_szDeviceName, "dx10") )
	//{
	//	return GutReleaseGraphicsDeviceDX10();
	//}

	return FALSE;
}
