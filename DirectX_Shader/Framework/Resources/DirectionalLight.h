#pragma once
#include "CLight.h"
#include "../Resources/CommonProcess.h"
//#include "CRenderInfo.h"

class DirectionalLight : public CLight
{

public:
	void Config()override;
	void Init()override {};
	void Uninit()override;
	void Update()override {};
};