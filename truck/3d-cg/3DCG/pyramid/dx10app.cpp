#include "stdafx.h"
#include "dx10app.h"
#include <d3d10.h>
#include <d3dx10.h>

#include "..\glib\Utility\Gut.h"

#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")

#include "render_data.h"

static ID3D10Device			*g_pDevice = NULL;
static ID3D10InputLayout		*g_pVertexLayout = NULL;
static ID3D10Buffer				*g_pVertexBuffer = NULL;
static ID3D10Buffer				*g_pConstantBuffer = NULL;
static ID3D10VertexShader	*g_pVertexShader = NULL;
static ID3D10PixelShader		*g_pPixelShader = NULL;

bool InitResourceDX10(void)
{
	g_pDevice = GutGetGraphicsDeviceDX10();
	ID3D10Blob *pVSCode = NULL;

	//載入vertex shader
	g_pVertexShader = GutLoadVertexShaderDX10_HLSL("vertex_transform_dx10.shader", "VS", "vs_4_0", &pVSCode);
	if(NULL == g_pVertexShader)
		return FALSE;
	//載入pixel shader
	g_pPixelShader = GutLoadPixelShaderDX10_HLSL("vertex_transform_dx10.shader", "PS", "ps_4_0");
	if(NULL == g_pPixelShader)
		return FALSE;

	//設定vertex 資料格式
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,D3D10_INPUT_PER_VERTEX_DATA, 0},
	};
	if(D3D_OK != g_pDevice->CreateInputLayout(layout, 1,pVSCode->GetBufferPointer(), pVSCode->GetBufferSize(), &g_pVertexLayout))
		return FALSE;

	SAFE_RELEASE(pVSCode);

	//設定一塊可以放Vertex的記憶體
	D3D10_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(g_vertices);
	cbDesc.Usage = D3D10_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;

	//配置一塊可以存放vertex的記憶體,也就是vertex buffer
	if(D3D_OK != g_pDevice->CreateBuffer(&cbDesc, NULL, &g_pVertexBuffer))
		return FALSE;

	//把資料拷貝到顯示卡記憶體的vertex buffer
	Vector4 *pVertexBuffer;
	g_pVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, NULL, (void **)&pVertexBuffer);
	memcpy(pVertexBuffer, g_vertices, sizeof(g_vertices));
	g_pVertexBuffer->Unmap();

	//配置shader讀取參數的記憶體空間
	cbDesc.ByteWidth = sizeof(Matrix4x4);
	cbDesc.Usage = D3D10_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	if(D3D_OK != g_pDevice->CreateBuffer(&cbDesc, NULL, &g_pConstantBuffer))
		return FALSE;

	//計算出一個可以轉換到鏡頭座標系的矩陣
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	//計算一個使用平行投影的矩陣
	Matrix4x4 perspective_matrix = GutMatrixPerspectiveRH_DirectX(90.f, 1.0f, 1.0f, 100.0f);
	//把這兩矩陣相乘
	Matrix4x4 view_perspective_matrix = view_matrix * perspective_matrix;

	//把矩陣拷貝到shader參數區的記憶體空間
	Matrix4x4 *pConstData;
	g_pConstantBuffer->Map(D3D10_MAP_WRITE_DISCARD, NULL, (void **)&pConstData);
	*pConstData = view_perspective_matrix;
	g_pConstantBuffer->Unmap();
	
	return TRUE;
}

void RenderFrameDX10(void)
{
	Vector4 vClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	UINT stride = sizeof(Vector4);
	UINT offset = 0;

	ID3D10RenderTargetView *pRenderTargetView = GutGetDX10RenderTargetView();
	ID3D10DepthStencilView *pDepthStencilView = GutGetDX10DepthStencilView();
	IDXGISwapChain *pSwapChain = GutGetDX10SwapChain();

	//清除顏色
	g_pDevice->ClearRenderTargetView(pRenderTargetView, (float *)&vClearColor);
	//清除depth stencil buffer
	g_pDevice->ClearDepthStencilView(pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
	//設定vertex shader
	g_pDevice->VSSetShader(g_pVertexShader);
	//設定pixel shader
	g_pDevice->PSSetShader(g_pPixelShader);
	//設定vertex shader讀取參數的記憶體配置
	g_pDevice->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	//設定vertex資料格式
	g_pDevice->IASetInputLayout(g_pVertexLayout);
	//設定vertex buffer
	g_pDevice->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	//設定要畫線
	g_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	//畫線
	g_pDevice->Draw(16, 0);
	//更新畫面
	pSwapChain->Present(0, 0);
}

bool ReleaseResourceDX10(void)
{
	SAFE_RELEASE(g_pDevice);
	SAFE_RELEASE(g_pVertexLayout);
	SAFE_RELEASE(g_pVertexBuffer);
	SAFE_RELEASE(g_pConstantBuffer);
	SAFE_RELEASE(g_pVertexShader);
	SAFE_RELEASE(g_pPixelShader);

	return TRUE;
}

