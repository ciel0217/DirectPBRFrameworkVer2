#include "Test3D.h"
#include "Framework/Manager/ManagerMaterial.h"
#include "Framework/Manager/ManagerInput.h"

void Test3D::Config()
{
	SetUpModel("Asset/model/Sphere.obj", this);
	MATERIAL_CBUFFER value;
	ZeroMemory(&value, sizeof(MATERIAL_CBUFFER));
	value.Metaric = 0;
	value.Roughness = 0;
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
	if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_RETURN))
	{
		m_Start = 1;
	}
	if (m_Start == 1)
	{
		m_Count++;
		if (m_Count > 30)
		{
			m_Count = 0;
			m_Count2++;
			MATERIAL_CBUFFER value;
			ZeroMemory(&value, sizeof(MATERIAL_CBUFFER));
			value.Metaric = float(m_Count2 % 10) / 10.0f;
			value.Roughness = float(m_Count3 % 10) / 10.0f;
			m_Material[0]->SetMaterialValue(value);
			if (m_Count2 % 10 == 0)m_Count3++;
		}
	}
}
