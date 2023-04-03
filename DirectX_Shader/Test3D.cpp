#include "Test3D.h"
#include "Framework/Manager/ManagerMaterial.h"
#include "Framework/Manager/ManagerInput.h"
#include "Framework/Resources/ObbCollision.h"

void Test3D::Config()
{
	SetUpModel("Asset/model/enemy.obj", this);
	m_Collision = new ObbCollision(this);
	

	MATERIAL_CBUFFER value;
	ZeroMemory(&value, sizeof(MATERIAL_CBUFFER));
	value.Metaric = 0.5f;
	value.Roughness = 0.5;
	value.UseAlbedoTex = 1;
	m_Material[0]->SetMaterialValue(value);
	m_Count = 0;
	m_Count2 = 0;
	m_Count3 = 0;
	m_Start = 0;
}

void Test3D::Init()
{
	m_Position = m_InitPosition;
	m_Scale = m_InitScale;
	m_Rotation = m_InitRotation;

	
}

void Test3D::Uninit()
{
}

void Test3D::Update()
{
	if (ManagerInput::GetInstance()->GetKeyboardPress(DIK_RETURN))
	{
		m_Position.y -= 0.5f;
	}
	m_Collision->UpdateCollision();
}
