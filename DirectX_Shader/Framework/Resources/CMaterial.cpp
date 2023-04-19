
#include "CMaterial.h"
#include "../Manager/ManagerShader.h"

void CMaterial::SetIntValue(std::string parameter_name, int value)
{
	//Int‚ÌParameter‚È‚¢‚íw
}

void CMaterial::SetFloatValue(std::string parameter_name, float value)
{
	if (parameter_name == "Roughness")
		m_MaterialValue.Roughness = value;
	else if (parameter_name == "Metaric")
		m_MaterialValue.Metaric = value;
	else if (parameter_name == "Specular")
		m_MaterialValue.Specular = value;
}

void CMaterial::SetColorValue(std::string parameter_name, D3DXCOLOR value)
{
	if (parameter_name == "BaseColor")
		m_MaterialValue.BaseColor = value;
}

void CMaterial::SetShader(std::string shader_name)
{
	m_Shader = ManagerShader::GetShader(shader_name);
}
