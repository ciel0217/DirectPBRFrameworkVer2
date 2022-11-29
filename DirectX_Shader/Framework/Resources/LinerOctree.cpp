#include "LinerOctree.h"
#include "CGameObject.h"
#include "CCollision.h"
#include "BoxCollision.h"

void LinerOctree::Init()
{
	m_UnitLen = (m_Max - m_Min) / (float)(1 << m_DimensionLevel);

	m_SpaceNum = (POWER_NUMBER[m_DimensionLevel + 1] - 1) / 7;
	m_ObjectList.reserve(m_SpaceNum);
}

void LinerOctree::CalcOctree(CGameObject * object)
{
	CCollision* collision = object->GetCollision();
	//�R���W�����̃^�C�v(BOX,Sphere,...etc)���擾
	CollisionType type = collision->GetCollisionType();

	D3DXVECTOR3 min, max;
	//�R���W�����^�C�v�ɂ���čŏ����W�A�ő���W�̂Ƃ�����Ⴄ
	switch (type)
	{
	case eBoxCollision:
		D3DXVECTOR3 size = ((BoxCollision*)collision)->GetCollisionBoxSize();
		D3DXVECTOR3 center = ((BoxCollision*)collision)->GetCollisionCenterPosition();

		min = center - size / 2.0f;
		max = center + size / 2.0f;
		break;
	case eCollisionTypeMax:
		break;
	default:
		break;
	}


}
