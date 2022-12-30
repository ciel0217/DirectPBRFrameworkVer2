#pragma once
#include "CGameObject.h"

class CParticle 
{
private:
	int m_FrameCount = 0;
	int m_LimitLifeTime = 10;
	int m_AnimationCountUp = 1;
	int m_AnimationCount = 0;

	bool m_IsLoopAnimation = false;
	bool m_IsDeath = true;
	bool m_UseAnime = false;

	D3DXVECTOR3 m_ParticlePosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ParticleScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR m_ParticleColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXQUATERNION m_ParticleRotation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVECTOR2 m_ParticleUV = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_TextureOffset = D3DXVECTOR2(1.0f, 1.0f);

	D3DXVECTOR3 m_ParticleInitPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ParticleInitScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_ParticleVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQUATERNION m_ParticleInitRotation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXCOLOR m_ParticleInitColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXVECTOR3 m_AddScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQUATERNION m_AddAngleByQuaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXCOLOR   m_AddColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	CGameObject* m_Parent;

	D3DXQUATERNION AngleToQuaternion(D3DXVECTOR3 angle);

public:
	CParticle() { m_IsDeath = true; }
	~CParticle(){}

	void Init();
	void Update();

	void SetParticlePosition(D3DXVECTOR3 pos) { m_ParticleInitPosition = pos; }
	void SetParticleScale(D3DXVECTOR3 scale) { m_ParticleInitScale = scale; }
	void SetParticleVelocity(D3DXVECTOR3 velo) { m_ParticleVelocity = velo; }
	void SetParticleRotation(D3DXQUATERNION qua) { m_ParticleInitRotation = qua; }
	void SetUV(D3DXVECTOR2 uv) { m_ParticleUV = uv; }
	void SetOffset(D3DXVECTOR2 uv) { m_TextureOffset = uv; }

	void SetAddScale(D3DXVECTOR3 scale) { m_AddScale = scale; }
	void SetAddAngle(D3DXVECTOR3 angle) { m_AddAngleByQuaternion = AngleToQuaternion(angle); }
	void SetAddColor(D3DXCOLOR color) { m_AddColor = color; }

	void SetParent(CGameObject* parent) { m_Parent = parent; }
	void SetLimitLifeTime(int time) { m_LimitLifeTime = time; }

	void SetAnimationCountUp(int count) { m_AnimationCountUp = count; }
	void SetIsLoopAnimation(bool is_loop) { m_IsLoopAnimation = is_loop; }
	void SetUseAnimation(bool use) { m_UseAnime = use; }

	bool GetIsDeath() { return m_IsDeath; }
	void SetIsDeath(bool is_death) { m_IsDeath = is_death; }

	D3DXVECTOR3 GetPosition() { return m_ParticlePosition; }
	D3DXVECTOR3 GetScale() { return m_ParticleScale; }
	D3DXQUATERNION GetRotation() { return m_ParticleRotation; }
	D3DXCOLOR GetColor() { return m_ParticleColor; }
	D3DXVECTOR2 GetUV() { return m_ParticleUV; }
	D3DXVECTOR2 GetOffset() { return m_TextureOffset; }

};