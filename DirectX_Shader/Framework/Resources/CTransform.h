#pragma once
#include "DirectX.h"
#include "ObjectTag.h"


class CTransform {
protected:
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQUATERNION m_Rotation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_Velocity= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 m_InitPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQUATERNION m_InitRotation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 m_InitScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_InitVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

public:
	CTransform(){}
	virtual ~CTransform(){}

	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXQUATERNION GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetScale() { return m_Scale; }
	D3DXVECTOR3 GetVelocity() { return m_Velocity; }

	void SetPosition(D3DXVECTOR3 pos) { m_Position = pos; }
	void SetRotation(D3DXQUATERNION qua) { m_Rotation = qua; }
	void SetScale(D3DXVECTOR3 scale) { m_Scale = scale; }
	void SetVelocity(D3DXVECTOR3 velo) { m_Velocity = velo; }

	void SetInitPosition(D3DXVECTOR3 pos) { m_InitPosition = pos; }
	void SetInitRotation(D3DXQUATERNION qua) { m_InitRotation = qua; }
	void SetInitScale(D3DXVECTOR3 scale) { m_InitScale = scale; }
	void SetInitVelocity(D3DXVECTOR3 velo) { m_InitVelocity = velo; }

};