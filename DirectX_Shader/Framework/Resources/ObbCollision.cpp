#include "ObbCollision.h"
#include "CGameObject.h"
#include "../../Math/Quaternion.h"

D3DXVECTOR3 ObbCollision::CalcAxis(D3DXVECTOR3 Unit)
{
	return Transform(Unit, m_Rotation);
}

void ObbCollision::UpdateCollision()
{
	m_CenterPosition = m_Self->GetPosition() + m_Offset;
	m_Rotation = m_Self->GetRotation();

	m_XAxis = CalcAxis(UNITX);
	m_YAxis = CalcAxis(UNITY);
	m_ZAxis = CalcAxis(UNITZ);
}
