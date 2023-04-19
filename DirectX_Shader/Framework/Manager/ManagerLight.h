#pragma once
//#include "CLight.h"
#include <list>
#include <memory>
#include "../Resources/StructuredBuffer.h"
#include "../Resources/CBuffer.h"
#include "../LowLevel/DirectX.h"

class StructuredBuffer;
class CLight;
class CBuffer;

class ManagerLight {
private:
	std::list<CLight*> m_Lights;
	D3DXCOLOR m_GlobalAmbient;
	std::unique_ptr<StructuredBuffer> m_LightSBuffer;
	std::unique_ptr<CBuffer> m_LightCBuffer;

public:
	ManagerLight();
	~ManagerLight();
	
	void Config();
	void Init();
	void Uninit();
	void Update();
	void SetLightCBuffer();

	void SetLight(CLight* light) {
		m_Lights.push_back(light);
	}

};