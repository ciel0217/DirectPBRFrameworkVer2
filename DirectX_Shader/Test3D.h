#pragma once
#include "Framework/Resources/CGameObject.h"
#include "Framework/Renderer/MeshRenderer.h"

class Test3D : public CGameObject, public MeshRenderer
{
private:
	int m_Count, m_Count2, m_Count3, m_Start;
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};