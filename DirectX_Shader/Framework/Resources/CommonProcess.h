#pragma once
#include "CTransform.h"

class CCollision;

class CommonProcess :  public CTransform
{
protected:
	bool m_IsDestroy = false;
	CCollision* m_Collision = nullptr;
	bool m_IsActive = true;
	bool m_IsPause = false;

public:
	CommonProcess() {};
	virtual ~CommonProcess() {};

	virtual void Config() = 0;
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;

	CCollision* GetCollision() { return m_Collision; }

	bool Destroy();
	bool Release();
	virtual void SetActive(bool active) { m_IsActive = active; }
	void SetDestroy(bool destroy) { m_IsDestroy = destroy; }
	void SetPause(bool pause) { m_IsPause = pause; }

};

