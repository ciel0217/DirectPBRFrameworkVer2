#pragma once
#include "ObjectTag.h"
#include "enum.h"

class CGameObject;

class CCollision
{

protected:
	bool m_Enable = true;
	int m_TagMask = TAG_EVERYTHING;
	CGameObject* m_Self = nullptr;

public:
	CCollision() = delete;
	CCollision(CGameObject* self) : m_Self(self) {};
	virtual ~CCollision() {};
	
	virtual void UpdateCollision() = 0;
	virtual CollisionType GetCollisionType() = 0;

	void SetEnable(bool enable) { m_Enable = enable; }
	bool GetEnable() { return m_Enable; }

	void SetTagMask(int mask) { m_TagMask = mask; }
	int GetTagMask() { return m_TagMask; }

	CGameObject* GetSelf() { return m_Self; }
};