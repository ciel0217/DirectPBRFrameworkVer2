#include "ParticleSystem.h"
#include "CParticle.h"


ParticleSystem::~ParticleSystem()
{
	for (int i = 0; i < m_ParticleList.size(); i++)
	{
		delete m_ParticleList[i];
	}
	m_ParticleList.clear();
	m_ParticleList.shrink_to_fit();
}

void ParticleSystem::Init()
{
	m_ParticleList.reserve(m_ParticleNum);
	m_FrameCount = 0;
}

void ParticleSystem::Update()
{
	m_FrameCount++;

	//�p�[�e�B�N���̐���
	if (m_FrameCount % m_GenerateTime == 0)
	{
		bool is_generate = false;
		for (auto particle : m_ParticleList)
		{
			if (particle->GetIsDeath())
			{
				particle->SetIsDeath(false);
				particle->SetParticlePosition(this->GetPosition());
				particle->Init();
				is_generate = true;

				break;
			}
		}

		if (!is_generate && m_ParticleList.size() < m_ParticleNum)
		{
			//�������A���P�[�^�Ƃ��g����������Ƃ悳����
			CParticle* particle = new CParticle();
			m_ParticleList.push_back(particle);

			InitParticle(particle);
			particle->Init();
		}
	}
	
	for (auto particle : m_ParticleList)
	{
		if(!particle->GetIsDeath())
			particle->Update();
	}
}

void ParticleSystem::InitParticle(CParticle* particle)
{
	particle->SetLimitLifeTime(m_LimitLifeTime);
	particle->SetParent(this);

	//�p�^�[���ŕ����Ă������B�����_���ɐ����Ƃ�
	particle->SetParticlePosition(this->GetPosition());
	particle->SetParticleRotation(m_ParticleRotation);
	particle->SetParticleScale(m_ParticleScale);
	particle->SetParticleVelocity(m_ParticleVelocity);

	particle->SetAddScale(m_AddScale);
	particle->SetAddColor(m_AddColor);
	particle->SetAddAngle(m_AddAngle);
}