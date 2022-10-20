#include "BoxCollision.h"
#include "../Resources/CGameObject.h"

void BoxCollision::UpdateCollision()
{
	m_CollisionCenterPosition = m_Self->GetPosition() + m_Offset;
}
