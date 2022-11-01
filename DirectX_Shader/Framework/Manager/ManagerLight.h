#pragma once
//#include "CLight.h"
#include <list>

class CBuffer;
class CLight;

class ManagerLight {
private:
	std::list<CLight*> m_Lights;
	D3DXCOLOR m_GlobalAmbient;
	CBuffer* m_LightCBuffer;

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