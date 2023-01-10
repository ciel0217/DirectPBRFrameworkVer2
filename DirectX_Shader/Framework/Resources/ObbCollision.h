#pragma once
#include "../Resources/CCollision.h"
#include "../LowLevel/DirectX.h"

class ObbCollision : public CCollision
{
private:
	D3DXVECTOR3 m_XAxis;
	D3DXVECTOR3 m_YAxis;
	D3DXVECTOR3 m_ZAxis;

	const D3DXVECTOR3 UNITX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 UNITY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3 UNITZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXVECTOR3 CalcAxis(D3DXVECTOR3 Unit);

protected:
	D3DXVECTOR3 m_CenterPosition;
	D3DXVECTOR3 m_Offset;
	D3DXVECTOR3 m_ObbSize = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXQUATERNION m_Rotation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

public:
	void UpdateCollision()override;
	CollisionType GetCollisionType()override { return eObbCollision; }

	D3DXVECTOR3 GetAxisX() { return m_XAxis; }
	D3DXVECTOR3 GetAxisY() { return m_YAxis; }
	D3DXVECTOR3 GetAxisZ() { return m_ZAxis; }

	D3DXVECTOR3 GetObbSize() { return m_ObbSize; }
	D3DXVECTOR3 GetCenterPosition() { return m_CenterPosition; }

};