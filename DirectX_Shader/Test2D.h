#pragma once
#include "Framework/Resources/CGameObject.h"
#include "Framework/Renderer/SpriteRenderer.h"

class Test2D : public CGameObject, public SpriteRenderer
{
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};