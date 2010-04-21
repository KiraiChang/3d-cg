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

	//���Jvertex shader
	g_pVertexShader = GutLoadVertexShaderDX10_HLSL("vertex_transform_dx10.shader", "VS", "vs_4_0", &pVSCode);
	if(NULL == g_pVertexShader)
		return FALSE;
	//���Jpixel shader
	g_pPixelShader = GutLoadPixelShaderDX10_HLSL("vertex_transform_dx10.shader", "PS", "ps_4_0");
	if(NULL == g_pPixelShader)
		return FALSE;

	//�]�wvertex ��Ʈ榡
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,D3D10_INPUT_PER_VERTEX_DATA, 0},
	};
	if(D3D_OK != g_pDevice->CreateInputLayout(layout, 1,pVSCode->GetBufferPointer(), pVSCode->GetBufferSize(), &g_pVertexLayout))
		return FALSE;

	SAFE_RELEASE(pVSCode);

	//�]�w�@���i�H��Vertex���O����
	D3D10_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(g_vertices);
	cbDesc.Usage = D3D10_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;

	//�t�m�@���i�H�s��vertex���O����,�]�N�Overtex buffer
	if(D3D_OK != g_pDevice->CreateBuffer(&cbDesc, NULL, &g_pVertexBuffer))
		return FALSE;

	//���ƫ�������ܥd�O���骺vertex buffer
	Vector4 *pVertexBuffer;
	g_pVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, NULL, (void **)&pVertexBuffer);
	memcpy(pVertexBuffer, g_vertices, sizeof(g_vertices));
	g_pVertexBuffer->Unmap();

	//�t�mshaderŪ���Ѽƪ��O����Ŷ�
	cbDesc.ByteWidth = sizeof(Matrix4x4);
	cbDesc.Usage = D3D10_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	if(D3D_OK != g_pDevice->CreateBuffer(&cbDesc, NULL, &g_pConstantBuffer))
		return FALSE;

	//�p��X�@�ӥi�H�ഫ�����Y�y�Шt���x�}
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	//�p��@�ӨϥΥ����v���x�}
	Matrix4x4 perspective_matrix = GutMatrixPerspectiveRH_DirectX(90.f, 1.0f, 1.0f, 100.0f);
	//��o��x�}�ۭ�
	Matrix4x4 view_perspective_matrix = view_matrix * perspective_matrix;

	//��x�}������shader�ѼưϪ��O����Ŷ�
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

	//�M���C��
	g_pDevice->ClearRenderTargetView(pRenderTargetView, (float *)&vClearColor);
	//�M��depth stencil buffer
	g_pDevice->ClearDepthStencilView(pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
	//�]�wvertex shader
	g_pDevice->VSSetShader(g_pVertexShader);
	//�]�wpixel shader
	g_pDevice->PSSetShader(g_pPixelShader);
	//�]�wvertex shaderŪ���Ѽƪ��O����t�m
	g_pDevice->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	//�]�wvertex��Ʈ榡
	g_pDevice->IASetInputLayout(g_pVertexLayout);
	//�]�wvertex buffer
	g_pDevice->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	//�]�w�n�e�u
	g_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	//�e�u
	g_pDevice->Draw(16, 0);
	//��s�e��
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

