#include "Test2D.h"
#include "Framework/LowLevel/CDxRenderer.h"

void Test2D::Config()
{
	
	ID3D11ShaderResourceView* depth = CDxRenderer::GetRenderer()->GetDepthStencilSRV();
	SetUpMaterial(depth, "Shader/2DTexture.hlsl", "Test2D");
	SetUpSprite(this);
}

void Test2D::Init()
{
	
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
}

void Test2D::Uninit()
{
}

void Test2D::Update()
{
}
