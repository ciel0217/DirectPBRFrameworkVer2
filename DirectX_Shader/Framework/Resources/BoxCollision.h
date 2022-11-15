#pragma once
#include "../Resources/CCollision.h"
#include "../LowLevel/DirectX.h"

class CGameObject;

class BoxCollision : public CCollision
{
private:
	D3DXVECTOR3 m_CollisoinBoxSize = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_CollisionCenterPosition;
	D3DXVECTOR3 m_Offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

public:
	BoxCollision() = delete;
	BoxCollision(CGameObject* self, D3DXVECTOR3 size) : CCollision(self) { m_CollisoinBoxSize = size; };
	BoxCollision(CGameObject* self, D3DXVECTOR3 size, D3DXVECTOR3 offset) : CCollision(self) { m_CollisoinBoxSize = size; m_Offset = offset; };
	virtual ~BoxCollision() {};

	void UpdateCollision()override;
	CollisionType GetCollisionType() override{ return eBoxCollision; }
	
	void SetCollisionBoxSize(D3DXVECTOR3 size) { m_CollisoinBoxSize = size; }
	D3DXVECTOR3 GetCollisionBoxSize() { return m_CollisoinBoxSize; }

	void SetCollisionCenterPosition(D3DXVECTOR3 pos) { m_CollisionCenterPosition = pos; }
	D3DXVECTOR3 GetCollisionCenterPosition() { return m_CollisionCenterPosition; }

	
};