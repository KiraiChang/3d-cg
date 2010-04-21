#include "stdafx.h"
#include "dx9_shader.h"
#include "render_data.h"
#include "..\glib\Utility\Gut.h"



//#############################################################
//########################no shader##############################
//#############################################################
/*
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->Clear(
		0, NULL,//�M����ӵe��
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,//�M���C���Zbuffer
		D3DCOLOR_RGBA(0, 0, 0, 0),//���C��M�e�¦�
		1.0f,//���Y�Ԩ�̻�
		0//��stencil buffer�]��0
		);

	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	//�p��@�ӨϥΥ����v���x�}
	Matrix4x4 perspective_matrix = GutMatrixPerspectiveRH_DirectX(90.f, 1.0f, 1.0f, 100.0f);
	//��o��x�}�ۭ�
	Matrix4x4 view_perspective_matrix = view_matrix * perspective_matrix;
	//��Ŷ������y���I�ഫ��ù��y�Шt��
	Vector4 vertices[16];
	for(int i =0; i<16 ;i++)
	{
		vertices[i] = g_vertices[i] * view_perspective_matrix;
		vertices[i] /= vertices[i].GetW();
	}

	device->BeginScene();
	device->SetFVF(D3DFVF_XYZ);
	device->DrawPrimitiveUP(D3DPT_LINELIST, 8, vertices, sizeof(Vector4));
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}
*/

//#############################################################
//######################have shader##############################
//#############################################################

static LPDIRECT3DVERTEXSHADER9				g_pVertexShaderDX9 = NULL;
static LPDIRECT3DPIXELSHADER9					g_pPixelShaderDX9 = NULL;
static LPDIRECT3DVERTEXDECLARATION9 g_pVertexShaderDecl = NULL;

bool InitResourceDX9(void)
{
	//���Jvertex shader
	g_pVertexShaderDX9 = GutLoadVertexShaderDX9_HLSL("vertex_transform_dx9.shader", "VS", "vs_1_1");
	if(NULL == g_pVertexShaderDX9)
		return FALSE;
	//���Jpixel shader
	g_pPixelShaderDX9 = GutLoadPixelShaderDX9_HLSL("vertex_transform_dx9.shader", "PS", "ps_2_0");
	if(NULL == g_pPixelShaderDX9)
		return FALSE;

	//�p��X�@�ӥi�H�ഫ�����Y�y�Шt���x�}
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	//�p��@�ӨϥΥ����v���x�}
	Matrix4x4 perspective_matrix = GutMatrixPerspectiveRH_DirectX(90.f, 1.0f, 1.0f, 100.0f);
	//��o��x�}�ۭ�
	Matrix4x4 view_perspective_matrix = view_matrix * perspective_matrix;

	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	//�]�w�����ഫ�x�}
	device->SetVertexShaderConstantF(0, (float *)&view_perspective_matrix, 4);
	return TRUE;
}

void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->Clear(
		0, NULL,//�M����ӵe��
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,//�M���C���Zbuffer
		D3DCOLOR_RGBA(0, 0, 0, 0),//���C��M�e�¦�
		1.0f,//���Y�Ԩ�̻�
		0//��stencil buffer�]��0
		);

	//�}�l�Uø�ϫ��O
	device->BeginScene();
	//�]�w��Ʈ榡
	device->SetFVF(D3DFVF_XYZW);

	//�]�wshader
	device->SetVertexShader(g_pVertexShaderDX9);
	device->SetPixelShader(g_pPixelShaderDX9);
	//�e�K���u
	device->DrawPrimitiveUP(D3DPT_LINELIST, 8, g_vertices, sizeof(Vector4));
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

bool ReleaseResourceDX9(void)
{
	if(g_pVertexShaderDX9)
	{
		g_pVertexShaderDX9->Release();
		g_pVertexShaderDX9 = NULL;
	}

	if(g_pPixelShaderDX9)
	{
		g_pPixelShaderDX9->Release();
		g_pPixelShaderDX9 = NULL;
	}

	return TRUE;
}
