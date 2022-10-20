#pragma once
#include "CTransform.h"
#include "../Manager/ManagerModel.h"
#include "ObjectTag.h"

class CBuffer;
class CCollision;

class CGameObject : public CTransform
{
protected:
	bool m_IsDestroy = false;
	CCollision* m_Collision = nullptr;
	bool m_IsActive = true;
	bool m_IsPause = false;
	int m_Tag = TAG_UNTAGGED;

public:
	CGameObject() {};
	virtual ~CGameObject()override { };

	virtual void Config() = 0;
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;

	virtual void OnCollisionEnter(CGameObject* collider) {};

	CCollision* GetCollision() { return m_Collision; }

	//ゲーム中に消す場合
	bool Destroy();
	//シーンの切り替えなどの絶対にリソース解放する場合
	bool Release();
	virtual void SetActive(bool active) { m_IsActive = active; }
	void SetDestroy(bool destroy) { m_IsDestroy = destroy; }
	void SetPause(bool pause) { m_IsPause = pause; }

	int GetTag() { return m_Tag; }
};