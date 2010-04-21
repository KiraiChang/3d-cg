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
		0, NULL,//清除整個畫面
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,//清除顏色跟Zbuffer
		D3DCOLOR_RGBA(0, 0, 0, 0),//把顏色清呈黑色
		1.0f,//鏡頭拉到最遠
		0//把stencil buffer設為0
		);

	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	//計算一個使用平行投影的矩陣
	Matrix4x4 perspective_matrix = GutMatrixPerspectiveRH_DirectX(90.f, 1.0f, 1.0f, 100.0f);
	//把這兩矩陣相乘
	Matrix4x4 view_perspective_matrix = view_matrix * perspective_matrix;
	//把空間中的座標點轉換到螢幕座標系統
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
	//載入vertex shader
	g_pVertexShaderDX9 = GutLoadVertexShaderDX9_HLSL("vertex_transform_dx9.shader", "VS", "vs_1_1");
	if(NULL == g_pVertexShaderDX9)
		return FALSE;
	//載入pixel shader
	g_pPixelShaderDX9 = GutLoadPixelShaderDX9_HLSL("vertex_transform_dx9.shader", "PS", "ps_2_0");
	if(NULL == g_pPixelShaderDX9)
		return FALSE;

	//計算出一個可以轉換到鏡頭座標系的矩陣
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	//計算一個使用平行投影的矩陣
	Matrix4x4 perspective_matrix = GutMatrixPerspectiveRH_DirectX(90.f, 1.0f, 1.0f, 100.0f);
	//把這兩矩陣相乘
	Matrix4x4 view_perspective_matrix = view_matrix * perspective_matrix;

	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	//設定視角轉換矩陣
	device->SetVertexShaderConstantF(0, (float *)&view_perspective_matrix, 4);
	return TRUE;
}

void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->Clear(
		0, NULL,//清除整個畫面
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,//清除顏色跟Zbuffer
		D3DCOLOR_RGBA(0, 0, 0, 0),//把顏色清呈黑色
		1.0f,//鏡頭拉到最遠
		0//把stencil buffer設為0
		);

	//開始下繪圖指令
	device->BeginScene();
	//設定資料格式
	device->SetFVF(D3DFVF_XYZW);

	//設定shader
	device->SetVertexShader(g_pVertexShaderDX9);
	device->SetPixelShader(g_pPixelShaderDX9);
	//畫八條線
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
