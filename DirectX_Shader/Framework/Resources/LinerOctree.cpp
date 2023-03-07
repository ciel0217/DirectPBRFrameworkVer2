#include "LinerOctree.h"
#include "CGameObject.h"
#include "CCollision.h"
#include "BoxCollision.h"
#include "ObbCollision.h"


LinerOctree::~LinerOctree()
{
	for (unsigned int i = 0; i < m_SpaceNum; i++)
	{
		m_ObjectList[i].clear();
		m_ObjectList[i].shrink_to_fit();
	}

	m_ObjectList.clear();
	m_ObjectList.shrink_to_fit();
}

void LinerOctree::Init()
{
	m_UnitLen = (m_Max - m_Min) / (float)(1 << m_DimensionLevel);

	if (m_DimensionLevel >= MAX_DIMENSION_LEVEL)
		return;

	m_SpaceNum = (POWER_NUMBER[m_DimensionLevel + 1] - 1) / 7;
	m_ObjectList.resize(m_SpaceNum);

	for (unsigned int i = 0; i < m_SpaceNum; i++)
	{
		m_ObjectList[i].reserve(100);
	}

}

void LinerOctree::ClearList()
{
	for (unsigned int i = 0; i < m_SpaceNum; i++)
		m_ObjectList[i].clear();
}

void LinerOctree::CalcOctree(CGameObject * object)
{
	CCollision* collision = object->GetCollision();

	if (!collision)
		return;

	//コリジョンのタイプ(BOX,Sphere,...etc)を取得
	CollisionType type = collision->GetCollisionType();

	D3DXVECTOR3 min, max;
	D3DXVECTOR3 size;
	D3DXVECTOR3 center;
	ObbCollision* obb;
	D3DXQUATERNION qua;
	D3DXVECTOR3 vertex[8];
	D3DXMATRIX mtx_world, mtx_scale, mtx_rot, mtx_trans;
	D3DXVECTOR4 world_pos;

	//コリジョンタイプによって最小座標、最大座標のとり方が違う
	switch (type)
	{
	case eBoxCollision:
		size = ((BoxCollision*)collision)->GetCollisionBoxSize();
		center = ((BoxCollision*)collision)->GetCollisionCenterPosition();

		min = center - size / 2.0f;
		max = center + size / 2.0f;
		break;
	case eObbCollision:
		obb = (ObbCollision*)collision;
		size = obb->GetObbSize();
		center = obb->GetCenterPosition();

		qua = obb->GetRotation();

		//Obbの各頂点座標を求める
		//top
		vertex[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
		vertex[1] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		vertex[2] = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		vertex[3] = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
		//bottom
		vertex[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		vertex[5] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
		vertex[6] = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		vertex[7] = D3DXVECTOR3(1.0f, -1.0f, -1.0f);

		
		D3DXMatrixIdentity(&mtx_world);

		//サイズ
		D3DXMatrixScaling(&mtx_scale, size.x, size.y, size.z);
		D3DXMatrixMultiply(&mtx_world, &mtx_world, &mtx_scale);

		//回転
		D3DXMatrixRotationQuaternion(&mtx_rot, &qua);
		D3DXMatrixMultiply(&mtx_world, &mtx_world, &mtx_rot);

		//移動
		D3DXMatrixTranslation(&mtx_trans, center.x, center.y, center.z);
		D3DXMatrixMultiply(&mtx_world, &mtx_world, &mtx_trans);

		D3DXMatrixTranspose(&mtx_world, &mtx_world);
		
		
		D3DXVec3Transform(&world_pos, &vertex[0], &mtx_world);
		min = D3DXVECTOR3(world_pos.x, world_pos.y, world_pos.z);

		D3DXVec3Transform(&world_pos, &vertex[1], &mtx_world);
		max = D3DXVECTOR3(world_pos.x, world_pos.y, world_pos.z);


		for (int i = 2; i < 8; i++)
		{
			D3DXVec3Transform(&world_pos, &vertex[i], &mtx_world);

			if (min.x > world_pos.x && min.y > world_pos.y && min.z > world_pos.z)
				min = D3DXVECTOR3(world_pos.x, world_pos.y, world_pos.z);

			if (max.x < world_pos.x && max.y < world_pos.y && max.z < world_pos.z)
				max = D3DXVECTOR3(world_pos.x, world_pos.y, world_pos.z);
		}

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


