#include "ManagerLight.h"
#include "CBuffer.h"
#include "struct.h"
#include "CDxRenderer.h"


ManagerLight::ManagerLight()
{
	if (!m_LightCBuffer) {
		m_LightCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(LIGHT_CBUFFER), D3D11_BIND_CONSTANT_BUFFER, nullptr));
	}
}

ManagerLight::~ManagerLight()
{
	delete m_LightCBuffer;
}

void ManagerLight::Config()
{
	for (auto light : m_Lights) {
		light->Config();
	}
}

void ManagerLight::Init()
{
	for (auto light : m_Lights) {
		light->Init();
	}
}

void ManagerLight::Uninit()
{
	for (auto light : m_Lights) {
		light->Uninit();
	}

	m_Lights.remove_if([](CGameObject* obj) {
		return obj->Release();
	});
	m_Lights.clear();
	
}

void ManagerLight::Update()
{
	for (auto light : m_Lights) {
		light->Update();
	}
}

void ManagerLight::SetLightCBuffer()
{
	LIGHT_CBUFFER light_cbuffer;
	light_cbuffer.GlobalAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	for (int i = 0; i < LIGHT_MAX; i++) {
		light_cbuffer.Lights[i].Status = LIGHT_DISABLE;
	}
	int count = 0;
	for (auto light : m_Lights) {
		light_cbuffer.Lights[count++] = light->GetLight();
	}

	m_LightCBuffer->UpdateBuffer(&light_cbuffer);
	m_LightCBuffer->PSSetCBuffer(4);

	m_Lights.clear();
}
