#include "../Manager/ManagerMaterial.h"
#include "CMaterial.h"
#include "CShader.h"


std::unordered_map<unsigned int, std::shared_ptr<CMaterial>> ManagerMaterial::m_MaterialList;


std::shared_ptr<CMaterial> ManagerMaterial::GetMaterial(unsigned int material_id)
{
	if (m_MaterialList.count(material_id) != 0) {
		return m_MaterialList[material_id];
	}
	return nullptr;
}

//�}�e���A�������ꏏ���Ɠ���Hash�l�ɂȂ����Ⴄ
unsigned int ManagerMaterial::CreateMaterial(int render, CShader * shader, ID3D11ShaderResourceView * texture, std::string name)
{
	CMaterial* material = new CMaterial(render, shader, texture, name);
	unsigned int hash = std::hash<std::string>()(name);
	bool can_set = SetMaterial(material, hash);

	return can_set ? hash : 0;
}

//�}�e���A�������ꏏ���Ɠ���Hash�l�ɂȂ����Ⴄ
unsigned int ManagerMaterial::CreateMaterial(int render, MATERIAL_CBUFFER material_value, CShader * shader, ID3D11ShaderResourceView * texture, std::string name)
{
	CMaterial* material = new CMaterial(render, material_value, shader, texture, name);
	unsigned int hash = std::hash<std::string>()(name);
	bool can_set = SetMaterial(material, hash);

	return can_set ? hash : 0;
}


bool ManagerMaterial::SetMaterial(CMaterial * material, unsigned int hash)
{
	if (m_MaterialList.count(hash) != 0)return false;

	m_MaterialList[hash].reset(material);
	return true;
}