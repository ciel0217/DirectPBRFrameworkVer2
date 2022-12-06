#include "DirectionalLight.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/CShader.h"

void DirectionalLight::Config()
{


	m_Position = D3DXVECTOR3(.0f, .0f, .0f);


	//lightèâä˙âª
	m_Light.Direction = D3DXVECTOR4(-1.0f, -1.0f, 1.0f, 1.0f);
	m_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, .0f);
	m_Light.Attenuation = 0.0f;
	m_Light.Diffuse = D3DXCOLOR(5.0f, 5.0f, 5.0f, 1.0f);
	m_Light.Position = D3DXVECTOR4(m_Position.x, m_Position.y, m_Position.z, 1.0f);
	m_Light.Status = LIGHT_ENABLED;
	m_Light.LightType = DIRECTIONAL_LIGHT;


}

void DirectionalLight::Uninit()
{
}



