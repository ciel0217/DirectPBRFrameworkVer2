#pragma once
#include "../Renderer/ParticleRenderer.h"
#include "CGameObject.h"

class CParticle;

class ParticleSystem : public ParticleRenderer, public CGameObject
{
private:
	std::vector<CParticle*> m_ParticleList;
	int m_FrameCount = 0;

protected:
	
	int m_GenerateTime = 10;
	int m_CoolTime = 1;
	int m_ParticleNum = 1;

	int m_LimitLifeTime = 10;

	D3DXVECTOR3 m_ParticleScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ParticleVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQUATERNION m_ParticleRotation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXCOLOR m_ParticleColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXVECTOR3 m_AddScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_AddAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR   m_AddColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	const D3DXVECTOR3  UnitX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	void InitParticle(CParticle* particle);
public:
	ParticleSystem() {};
	virtual ~ParticleSystem();

	virtual void Config()override = 0;
	void Init()override;
	void Update()override;
	void Uninit()override;

	std::vector<CParticle*> GetParticleList() { return m_ParticleList; }
};