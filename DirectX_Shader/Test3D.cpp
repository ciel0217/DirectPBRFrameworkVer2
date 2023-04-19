#include "Test3D.h"
#include "Framework/Manager/ManagerMaterial.h"
#include "Framework/Manager/ManagerInput.h"
#include "Framework/Resources/ObbCollision.h"

void Test3D::Config()
{
	std::vector<std::string> materials_name;
	materials_name.push_back({ "enemy:lambert2SG" });
	SetUpModel("Asset/model/enemy.obj", materials_name);
	m_Collision = new ObbCollision(this);

	m_Materials[0]->SetFloatValue("Metaric", 0.5f);
	m_Materials[0]->SetFloatValue("Roughness", 0.5f);

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
