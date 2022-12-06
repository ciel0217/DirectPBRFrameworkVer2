#include "GameSceneCamera.h"

void GameSceneCamera::Init()
{
	m_LookAtPoint = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	m_CameraUp = UnitY;
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void GameSceneCamera::Uninit()
{
}
