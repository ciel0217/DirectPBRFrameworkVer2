#include "CParticle.h"

#define FRAME_RATE 60.0f

D3DXQUATERNION CParticle::AngleToQuaternion(D3DXVECTOR3 angle)
{
	float cr = cosf(angle.x * 0.5f);
	float sr = sinf(angle.x * 0.5f);
	float cp = cosf(angle.y * 0.5f);
	float sp = sinf(angle.y * 0.5f);
	float cy = cosf(angle.z * 0.5f);
	float sy = cosf(angle.z * 0.5f);

	D3DXQUATERNION ret = D3DXQUATERNION(
		sr * cp * cy - cr * sp * sy,
		cr * sp * cy + sr * cp * sy,
		cr * cp * sy - sr * sp * cy,
		cr * cp * sy - sr * sp * cy
	);

	return ret;
}

void CParticle::Init()
{
	m_ParticlePosition = m_ParticleInitPosition;
	m_ParticleColor = m_ParticleInitColor;
	m_ParticleRotation = m_ParticleInitRotation;
	m_ParticleScale = m_ParticleInitScale;

	m_FrameCount = 0;
	m_AnimationCount = 0;
	
}

void CParticle::Update()
{
	m_FrameCount++;
			   
	if (m_FrameCount > m_LimitLifeTime * FRAME_RATE)
	{
		m_IsDeath = true;
		m_FrameCount = 0;
		m_AnimationCount = 0;
	}

	if (m_FrameCount % m_AnimationCountUp == 0 && m_UseAnime)
	{
		m_AnimationCount++;
		//��(x), �c(y)�ɉ������邩
		int x_tiling = (int)(1.0f / m_TextureOffset.x);
		int y_tiling = (int)(1.0f / m_TextureOffset.y);

		m_ParticleUV.x = m_AnimationCount % x_tiling * m_TextureOffset.x;
		m_ParticleUV.y = m_AnimationCount / y_tiling * m_TextureOffset.y;

		if (!m_IsLoopAnimation)
		{
			if (m_AnimationCount >= x_tiling * y_tiling)
			{
				m_ParticleUV.x = 1.0f - m_TextureOffset.x;
				m_ParticleUV.y = 1.0f - m_TextureOffset.y;
			}
		}
	}

	m_ParticlePosition += m_ParticleVelocity / FRAME_RATE;
	m_ParticleScale += m_AddScale / FRAME_RATE;
	m_ParticleRotation += m_AddAngleByQuaternion / FRAME_RATE;
	m_ParticleColor += m_AddColor / FRAME_RATE;

	
}
