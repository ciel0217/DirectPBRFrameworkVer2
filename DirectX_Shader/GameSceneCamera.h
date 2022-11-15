#pragma once
#include "Framework/Resources/Camera.h"

class GameSceneCamera : public Camera
{
public:
	void Init()override;
	void Uninit()override;
};