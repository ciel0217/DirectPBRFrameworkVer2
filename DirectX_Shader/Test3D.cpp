#include "Test3D.h"
#include "Framework/Manager/ManagerMaterial.h"

void Test3D::Config()
{
	SetUpModel("Asset/model/Cube.obj", this);
	MATERIAL_CBUFFER value;
	ZeroMemory(&value, sizeof(MATERIAL_CBUFFER));
	value.Metaric = 0.5f;
	value.Roughness = 0.5f;
	ManagerMaterial::GetMaterial(m_MaterialIds[0])->SetMaterialValue(value);
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
}
