#include "CParticle.h"

#define FRAME_RATE 60.0f

D3DXQUATERNION CParticle::AngleToQuaternion(D3DXVECTOR3 angle)
{
	return D3DXQUATERNION();
}

void CParticle::Init()
{
	m_ParticlePosition = m_ParticleInitPosition;
	m_ParticleColor = m_ParticleInitColor;
	m_ParticleRotation = m_ParticleInitRotation;
	m_ParticleScale = m_ParticleInitScale;

	m_FrameCount = 0;
}

void CParticle::Update()
{
	m_FrameCount++;
			   
	if (m_FrameCount > m_LimitLifeTime * FRAME_RATE)
	{
		m_IsDeath = true;
	}

	m_ParticlePosition += m_ParticleVelocity / FRAME_RATE;
	m_ParticleScale += m_AddScale / FRAME_RATE;
	m_ParticleRotation += m_AddAngleByQuaternion / FRAME_RATE;
	m_ParticleColor += m_AddColor / FRAME_RATE;
}
