#include "GutCamera.h"

#include "..\utility\Gut.h"

//GutCamera::GutCamera(void)
//{
//// 鏡頭位置
//m_eye = Vector4(0.0f, 0.5f, 10.0f); 
//// 鏡頭對準的點
//m_lookat = Vector4(0.0f, 0.5f, 0.0f); 
//// 鏡頭正上方的方向
//m_up = Vector4(0.0f, 1.0f, 0.0f);
//}
//
//GutCamera::~GutCamera(void)
//{
//
//}

Matrix4x4 GutCamera::GetViewMatrix()
{
	return GutMatrixLookAtRH(m_eye, m_lookat, m_up);
}

void GutCamera::SetFacing(float phi, float theta)
{
	////計算目前鏡頭面向
	//m_face = m_lookat - m_eye;
	////確保它是個單位向量
	//m_face.Normalize();

	float sin_phi, cos_phi;
	float sin_theta, cos_theta;
	
	FastMath::SinCos(phi, sin_phi, cos_phi);
	FastMath::SinCos(theta, sin_theta, cos_theta);

	//計算鏡頭的面向
	m_face[0] = cos_phi * cos_theta;
	m_face[1] = sin_phi;
	m_face[2] = cos_phi * sin_theta;

	//計算鏡頭正上方的軸向
	FastMath::SinCos(phi + MATH_PI*0.5f, sin_phi, cos_phi);

	m_top[0] = cos_phi * cos_theta;
	m_top[1] = sin_phi;
	m_top[2] = cos_phi * sin_theta;

	//取得鏡面右方的方向
	m_right = Vector3CrossProduct(m_face, m_top);
}

void GutCamera::SetCamera(Vector4 &eye, Vector4 &lookat, Vector4 &up)
{
	m_eye = eye;
	m_lookat = lookat;
	m_up = up;
}

void GutCamera::Move(float move)
{
	m_eye += m_face * move;
}
/*
void GutCamera::Rotation(float speed)
{
	if ( speed )
	{
		//先取得一個旋轉矩陣
		Matrix4x4 rotate_matrix;
		rotate_matrix.RotateY_Replace(-speed);
		//把原本的面向沿Y軸旋轉ry度, 取得新的面向.
		m_face = m_face * rotate_matrix;
	}
}
*/
void GutCamera::SetLookUp(void)
{
	m_lookat = m_eye + m_face;
	m_up = m_top;
}

void GutCamera::Yaw(float x)
{
	m_eye += m_right * x;
}

void GutCamera::Pitch(float y)
{
	m_eye += m_top * y;
}

void GutCamera::Roll(float z)
{

}