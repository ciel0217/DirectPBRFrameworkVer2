#include "LinerOctree.h"

void LinerOctree::Init()
{
	m_UnitLen = (m_Max - m_Min) / (float)(1 << m_DimensionLevel);

	m_SpaceNum = (POWER_NUMBER[m_DimensionLevel + 1] - 1) / 7;
	m_ObjectList.reserve(m_SpaceNum);
}

void LinerOctree::CalcOctree(CGameObject * object)
{
}
