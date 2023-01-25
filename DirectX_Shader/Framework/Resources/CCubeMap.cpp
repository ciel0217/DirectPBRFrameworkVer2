#include "CCubeMap.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Manager/ManagerTexture.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/CShader.h"
#include "../Manager/ManagerShader.h"
#include "CBuffer.h"



void CCubeMap::CreateCubeMapResourveAndView()
{

	m_Texture = DirectXTexture2D::CreateTextureCube(
		CUBE_MAP_SIZE, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, m_MipMapLevel);

	m_Texture->CreateSRV(DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_SRV_DIMENSION_TEXTURECUBE, m_MipMapLevel);
	m_Texture->CreateTextureCubeRTVs(DXGI_FORMAT_R16G16B16A16_FLOAT, m_MipMapLevel);
	
	m_ViewPort.Width = CUBE_MAP_SIZE;
	m_ViewPort.Height = CUBE_MAP_SIZE;
	m_ViewPort.TopLeftX = 0.0f;
	m_ViewPort.TopLeftY = 0.0f;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	m_CameraCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(CAMERA_CBUFFER), D3D11_BIND_CONSTANT_BUFFER, nullptr));
}

void CCubeMap::Init()
{
	CreateCubeMapResourveAndView();
	m_Position = D3DXVECTOR3(.0f, .0f, .0f);
}

void CCubeMap::Uninit()
{
	delete m_CameraCBuffer;
}

void CCubeMap::Update()
{


}


void CCubeMap::SetViewPort()
{
	CDxRenderer::GetRenderer()->GetDeviceContext()->RSSetViewports(1, &m_ViewPort);
}

void CCubeMap::SetCBuffer()
{
	//D3DXVECTOR3 lookpt, up;
	////+X–Ê
	//lookpt = m_Position + D3DXVECTOR3(1.0f, .0f, .0f);
	//up = D3DXVECTOR3(.0f, 1.0f, .0f);
	//D3DXMatrixLookAtLH(&m_CBufferCubeMap.View[0], &m_Position, &lookpt, &up);

	////-X–Ê
	//lookpt = m_Position + D3DXVECTOR3(-1.0f, .0f, .0f);
	//up = D3DXVECTOR3(.0f, 1.0f, .0f);
	//D3DXMatrixLookAtLH(&m_CBufferCubeMap.View[1], &m_Position, &lookpt, &up);

	////+Y–Ê
	//lookpt = m_Position + D3DXVECTOR3(.0f, 1.0f, .0f);
	//up = D3DXVECTOR3(.0f, .0f, 1.0f);
	//D3DXMatrixLookAtLH(&m_CBufferCubeMap.View[2], &m_Position, &lookpt, &up);


	////-Y–Ê
	//lookpt = m_Position + D3DXVECTOR3(.0f, -1.0f, .0f);
	//up = D3DXVECTOR3(.0f, .0f, -1.0f);
	//D3DXMatrixLookAtLH(&m_CBufferCubeMap.View[3], &m_Position, &lookpt, &up);


	////+Z–Ê
	//lookpt = m_Position + D3DXVECTOR3(.0f, .0f, 1.0f);
	//up = D3DXVECTOR3(.0f, 1.0f, .0f);
	//D3DXMatrixLookAtLH(&m_CBufferCubeMap.View[4], &m_Position, &lookpt, &up);


	////-Z–Ê
	//lookpt = m_Position + D3DXVECTOR3(.0f, .0f, -1.0f);
	//up = D3DXVECTOR3(.0f, 1.0f, .0f);
	//D3DXMatrixLookAtLH(&m_CBufferCubeMap.View[5], &m_Position, &lookpt, &up);

	//for (int i = 0; i < 6; i++) {
	//	D3DXMatrixTranspose(&m_CBufferCubeMap.View[i], &m_CBufferCubeMap.View[i]);
	//}

	//D3DXMatrixPerspectiveFovLH(&m_CBufferCubeMap.Projection, 1.57908f, 1.0f, 1.0f, 100000.0f);
	//CDxRenderer::GetRenderer()->GetDeviceContext()->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &m_CBufferCubeMap, 0, 0);

	
}

void CCubeMap::SetEnvMapCamera(int index)
{
	D3DXMATRIX mtxView, mtxProjection;
	D3DXVECTOR3 lookpt;
	lookpt = m_Position + forward[index];

	CAMERA_CBUFFER camera_cbuffer;
	ZeroMemory(&camera_cbuffer, sizeof(CAMERA_CBUFFER));

	D3DXMatrixLookAtLH(&mtxView, &m_Position, &lookpt, &up[index]);
	D3DXMatrixTranspose(&mtxView, &mtxView);
	camera_cbuffer.View = mtxView;
	//CDxRenderer::GetRenderer()->SetViewMatrix(&mtxView);

	D3DXMatrixPerspectiveFovLH(&mtxProjection, 1.57908f, 1.0f, 10.0f, 10000000.0f);
	D3DXMatrixTranspose(&mtxProjection, &mtxProjection);
	camera_cbuffer.Projection = mtxProjection;

	m_CameraCBuffer->UpdateBuffer(&camera_cbuffer);
	m_CameraCBuffer->VSSetCBuffer(1);
	m_CameraCBuffer->PSSetCBuffer(1);

	//CDxRenderer::GetRenderer()->SetProjectionMatrix(&mtxProjection);

}
