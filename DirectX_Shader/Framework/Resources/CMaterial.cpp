
#include "CMaterial.h"
#include "../Manager/ManagerShader.h"

void CMaterial::SetShader(std::string shader_name)
{
	m_Shader = ManagerShader::GetShader(shader_name);
}
