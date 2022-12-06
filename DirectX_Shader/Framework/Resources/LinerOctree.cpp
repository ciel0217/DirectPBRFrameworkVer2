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

void LinerOctree::ClearList()
{
	for (auto list : m_ObjectList)
	{
		list.clear();
	}
	m_ObjectList.clear();
}

void LinerOctree::CalcOctree(CGameObject * object)
{
	CCollision* collision = object->GetCollision();
	//コリジョンのタイプ(BOX,Sphere,...etc)を取得
	CollisionType type = collision->GetCollisionType();

	D3DXVECTOR3 min, max;
	D3DXVECTOR3 size;
	D3DXVECTOR3 center;
	//コリジョンタイプによって最小座標、最大座標のとり方が違う
	switch (type)
	{
	case eBoxCollision:
		size = ((BoxCollision*)collision)->GetCollisionBoxSize();
		center = ((BoxCollision*)collision)->GetCollisionCenterPosition();

		min = center - size / 2.0f;
		max = center + size / 2.0f;
		break;
	case eCollisionTypeMax:
		break;
	default:
		break;
	}

	DWORD elem_min = GetPointElem(min);
	DWORD elem_max = GetPointElem(max);

	unsigned int i = 0;
	unsigned int shift = 0;
	DWORD xor = elem_max ^ elem_min; //親空間を計算
	unsigned int space_index;

	while (xor != 0)
	{
		if ((xor & 0x7) != 0)
		{
			space_index = i + 1;
			shift = space_index * 3;
		}
		
		xor >>= 3;
		i++;
	}

	DWORD space_num = elem_max >> shift;
	DWORD add_num = (POWER_NUMBER[m_DimensionLevel - space_index] -1) / 7;
	space_num += add_num; //配列の要素番号

	//失敗
	if (space_num > m_SpaceNum)
		return;
	m_ObjectList[space_num].push_back(object);

}
