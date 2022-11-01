#pragma once
#include "../Resources/CGameObject.h"
#include "ObjectTag.h"
#include <list>
#include "../Resources/DevelopEnum.h"
#include "../Resources/CPostEffect.h"
#include "../Renderer/DeferredRenderer.h"
#include "SkyBox.h"
#include "../Renderer/PostEffectToneMap.h"
#include "../Manager/ManagerLight.h"
#include "../Renderer/CameraRenderer.h"


class CBuffer;
class CLight;

class Camera : public CGameObject, public CameraRenderer {
protected:
	D3DXVECTOR3 m_LookAtPoint;
	D3DXVECTOR3 m_CameraUp;

	const D3DXVECTOR3  UnitX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3  UnitY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3  UnitZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

public:

	Camera() { }
	virtual ~Camera() {};

	virtual void Config()override { SetUpRenderer(); }
	virtual void Init()override {};
	virtual void Uninit()override {};
	virtual void Update()override { SetVPCBuffer(m_Position, m_LookAtPoint, m_CameraUp); }


	D3DXVECTOR3 GetCameraLookAtPoint() { return m_LookAtPoint; }
	D3DXVECTOR3 GetCameraUp() { return m_CameraUp; }

};