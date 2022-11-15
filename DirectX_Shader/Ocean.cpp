#include "Ocean.h"
#include "Framework/Manager/ManagerTexture.h"
#include "Framework/Resources/CBuffer.h"
#include "Framework/LowLevel/CDxRenderer.h"
#include "Framework/Resources/CShader.h"

void Ocean::Config()
{
	m_NormalMapTextureName = "asset/texture/NormalMap.png";
	m_NormalTexture = ManagerTexture::LoadTexture(m_NormalMapTextureName);

	//m_ModelName = "asset/model/plane.obj";
	//m_Model = ManagerModel::Load(m_ModelName);
	////D3DXCOLOR basecolor = D3DXCOLOR(0.1373f, 0.6627f, 0.8784f, 1.0f);
	//D3DXCOLOR basecolor = D3DXCOLOR(0.0196f, 0.1349f, 0.4745f, 1.0f);
	//m_Model->SetBaseColor(basecolor);
	//m_Model->SetNormalTexture(m_NormalTexture);
	//m_Model->SetMetaric(0.0f);
	//m_Model->SetRoughness(0.2f);
	//m_Model->SetSpecular(1.0f);


	m_CBufferValue.Time = 0.0f;
	m_CBufferValue.WaveSpeed = 0.1f;
	//m_CBufferValue.MixColor = D3DXCOLOR(0.0353f, 0.2392f, 0.3216f, 1.0f);
	m_CBufferValue.MixColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	m_CBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(OCEAN_CBUFFER), D3D11_BIND_CONSTANT_BUFFER, nullptr));
}

void Ocean::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
}

void Ocean::Uninit()
{
}

void Ocean::Update()
{
	m_CBufferValue.Time += 0.1f;
}


