#include "BoxCollision.h"
#include "CGameObject.h"

void BoxCollision::UpdateCollision()
{
	m_CollisionCenterPosition = m_Self->GetPosition() + m_Offset;
}
