#pragma once
#include "../Renderer/ParticleRenderer.h"
#include "CGameObject.h"

class CParticle;

class ParticleSystem : public ParticleRenderer, public CGameObject
{
protected:
	std::vector<CParticle*> m_ParticleList;
	CGameObject* m_Parent;

	int m_FrameCount = 0;
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
public:
	virtual void Config()override = 0;
	virtual void Init()override = 0;
	virtual void Update()override = 0;

	void SetParticleScale(D3DXVECTOR3 scale) { m_ParticleScale = scale; }
	void SetParticleVelocity(D3DXVECTOR3 velo) { m_ParticleVelocity = velo; }
	void SetParticleRotation(D3DXQUATERNION qua) { m_ParticleRotation = qua; }

	void SetAddScale(D3DXVECTOR3 scale) { m_AddScale = scale; }
	void SetAddAngle(D3DXVECTOR3 angle) { m_AddAngle = angle; }
	void SetAddColor(D3DXCOLOR color) { m_AddColor = color; }

	void SetLimitLifeTime(int time) { m_LimitLifeTime = time; }
	void SetGenerateTime(int time) { m_GenerateTime = time; }

	void SetParent(CGameObject* parent) { m_Parent = parent; }
};