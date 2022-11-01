#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include "../LowLevel/DirectX.h"
#include "../Resources/DevelopStruct.h"

class CMaterial;
class CShader;

class ManagerMaterial {
private:
	static std::unordered_map<unsigned int , std::shared_ptr<CMaterial>> m_MaterialList;
    static bool SetMaterial(CMaterial* material, unsigned int );

public:
	ManagerMaterial(){}
	~ManagerMaterial() {}

	static std::shared_ptr <CMaterial> GetMaterial(unsigned int material_id);
	static unsigned int CreateMaterial(int render, CShader* shader, ID3D11ShaderResourceView* texture, std::string name);
	static unsigned int CreateMaterial(int render, MATERIAL_CBUFFER material_value,CShader* shader, ID3D11ShaderResourceView* texture, std::string name);


};