#ifndef _GUT_CAMERA_H_
#define _GUT_CAMERA_H_

#include "..\math\Vector4.h"
#include "..\math\Matrix4x4.h"

struct GutCamera
{
Vector4 m_eye;// 鏡頭位置
Vector4 m_lookat;// 鏡頭對準的點
Vector4 m_up;// 鏡頭正上方的方向

Vector4 m_face;
Vector4 m_right;
Vector4 m_top;
public:
	//Camera(void);
	//virtual ~Camera(void);
	Matrix4x4 GetViewMatrix(void);
	void SetFacing(float phi, float theta);
	void SetCamera(Vector4 &eye, Vector4 &lookat, Vector4 &up);
	void Move(float move);
	void SetLookUp(void);
	//void Rotation(float speed);
	void Yaw(float x);
	void Pitch(float y);
	void Roll(float z);
};

#endif //_GUT_CAMERA_H_