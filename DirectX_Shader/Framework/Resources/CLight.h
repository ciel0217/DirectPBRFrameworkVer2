#pragma once
#include "struct.h"
#include "CTransform.h"
#include "CGameObject.h"

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#define LIGHT_DISABLE 0
#define LIGHT_ENABLED 1
#define LIGHT_ENABLED_SHADOW 2

class CLight : public CGameObject{
protected:
	LIGHT m_Light;
	int m_LightNumber;

public:
	static int m_LightCount;
	CLight() { m_LightNumber = m_LightCount; m_LightCount++; }
	virtual ~CLight() { m_LightCount--; };

	virtual void Config()override {};
	virtual void Init()override {};
	virtual void Uninit()override {};
	virtual void Update()override {};

	int GetLightNumber() { return m_LightNumber; }
	LIGHT GetLight() { return m_Light; }
};
