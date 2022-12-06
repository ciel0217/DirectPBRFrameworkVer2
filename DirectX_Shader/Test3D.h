#pragma once
#include "Framework/Resources/CGameObject.h"
#include "Framework/Renderer/MeshRenderer.h"

class Test3D : public CGameObject, public MeshRenderer
{
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};