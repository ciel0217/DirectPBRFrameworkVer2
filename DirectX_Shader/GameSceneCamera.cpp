#include "GameSceneCamera.h"
#include "Framework/Manager/ManagerInput.h"

void GameSceneCamera::Init()
{
	m_LookAtPoint = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	m_CameraUp = UnitY;
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void GameSceneCamera::Uninit()
{
}

void GameSceneCamera::Update()
{
	if (ManagerInput::GetInstance()->GetKeyboardPress(DIK_A))
		m_Position.z -= 0.5f;
	SetVPCBuffer(m_Position, m_LookAtPoint, m_CameraUp);
}
