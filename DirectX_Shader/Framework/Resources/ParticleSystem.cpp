#include "ParticleSystem.h"
#include "CParticle.h"


ParticleSystem::~ParticleSystem()
{
	for (unsigned int i = 0; i < m_ParticleList.size(); i++)
	{
		delete m_ParticleList[i];
	}
	m_ParticleList.clear();
	m_ParticleList.shrink_to_fit();
}

void ParticleSystem::Init()
{
	m_Position = m_InitPosition;
	
	m_ParticleList.reserve(m_ParticleNum);
	m_FrameCount = 0;

	for (int i = 0; i < m_ParticleNum; i++)
	{
		CParticle* particle = new CParticle();
		m_ParticleList.push_back(particle);
		InitParticle(particle);
		particle->Init();
	}

	CreateStructuredBuffer(10);
}

void ParticleSystem::Update()
{
	m_FrameCount++;

	//パーティクルの生成
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

		//if (!is_generate && (int)m_ParticleList.size() < m_ParticleNum)
		//{
		//	//メモリアロケータとか使ったらもっとよさそう
		//	CParticle* particle = new CParticle();
		//	m_ParticleList.push_back(particle);

		//	InitParticle(particle);
		//	particle->Init();
		//}
	}
	
	for (auto particle : m_ParticleList)
	{
		if(!particle->GetIsDeath())
			particle->Update();
	}
}

void ParticleSystem::Uninit()
{
}

void ParticleSystem::InitParticle(CParticle* particle)
{
	particle->SetLimitLifeTime(m_LimitLifeTime);
	particle->SetParent(this);

	particle->SetIsDeath(false);

	//パターンで分けてもいい。ランダムに生成とか
	particle->SetParticlePosition(this->GetPosition());
	particle->SetParticleRotation(m_ParticleRotation);
	particle->SetParticleScale(m_ParticleScale);
	particle->SetParticleVelocity(m_ParticleVelocity);

	particle->SetAddScale(m_AddScale);
	particle->SetAddColor(m_AddColor);
	particle->SetAddAngle(m_AddAngle);

	particle->SetAnimationCountUp(m_AnimationCountUp);
	particle->SetIsLoopAnimation(m_IsLoopAnimation);
	particle->SetUseAnimation(m_UseAnimation);
	particle->SetUV(m_ParticleUV);
	particle->SetOffset(D3DXVECTOR2(1.0f / m_TextureMaxWidth, 1.0f / m_TextureMaxHeight));
}
