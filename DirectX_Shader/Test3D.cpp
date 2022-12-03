#include "Test3D.h"

void Test3D::Config()
{
	SetUpModel("Asset/model/Cube.obj", this);
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
