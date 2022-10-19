#pragma once
#include "CScene.h"
class TestScene : public CScene
{
public:
	TestScene() { SetObject(); }
	void SetObject()override;
};

