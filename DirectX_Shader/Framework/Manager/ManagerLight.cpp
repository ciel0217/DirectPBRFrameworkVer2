#include "../Manager/ManagerLight.h"

#include "../Resources/DevelopStruct.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/CLight.h"


ManagerLight::ManagerLight()
{
	if (!m_LightSBuffer) 
		m_LightSBuffer.reset(StructuredBuffer::CreateStructuredBuffer(sizeof(LIGHT), 10));
	

	if (!m_LightCBuffer) 
		m_LightCBuffer.reset(new CBuffer(CBuffer::CreateBuffer(sizeof(LIGHT_CBUFFER), D3D11_BIND_CONSTANT_BUFFER, nullptr)));
	
}

ManagerLight::~ManagerLight()
{
	
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
	light_cbuffer.LightCount = CLight::m_LightCount;

	m_LightCBuffer->UpdateBuffer(&light_cbuffer);
	m_LightCBuffer->VSSetCBuffer(4);
	m_LightCBuffer->PSSetCBuffer(4);

	std::vector<LIGHT> lights;
	int count = 0;
	for (auto light : m_Lights) {
		lights.push_back(light->GetLight());
	}

	m_LightSBuffer->UpdateBuffer(lights.data(), light_cbuffer.LightCount);
	m_LightSBuffer->PSSetStructuredBuffer(12);

	m_Lights.clear();
}
