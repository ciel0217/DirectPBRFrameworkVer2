#include "TestAnimation.h"

void TestAnimation::Config()
{
	MATERIAL_CBUFFER material;
	ZeroMemory(&material, sizeof(MATERIAL_CBUFFER));
	material.BaseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.UseAlbedoTex = 1;
	SetUpMaterial(eTransparent, "test", "Shader/ParticleRender.hlsl", "Asset/texture/enter.png", material);

	m_ParticleNum = 1;
	m_ParticleScale = D3DXVECTOR3(50.0f, 5.0f, 5.0f);
	m_LimitLifeTime = 10;
}
