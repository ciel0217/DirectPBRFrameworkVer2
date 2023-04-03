#include "TestAnimation.h"

void TestAnimation::Config()
{
	MATERIAL_CBUFFER material;
	ZeroMemory(&material, sizeof(MATERIAL_CBUFFER));
	material.BaseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.UseAlbedoTex = 1;
	SetUpMaterial(eTransparent, "test", "Shader/ParticleRender.hlsl", "asset/texture/enter.png", material);

	m_Self = this;
	m_ParticleNum = 1000;
	m_ParticleScale = D3DXVECTOR3(40.0f, 5.0f, 5.0f);
	m_ParticleVelocity = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	m_LimitLifeTime = 10;
	m_GenerateTime = 60;
}
