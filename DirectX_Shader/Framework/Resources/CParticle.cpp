#include "CParticle.h"


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
}
