#pragma once
#include <map>
#include <string>
#include "../LowLevel/DirectX.h"
#include "../Resources/DevelopStruct.h"

#define SHADER_MAX 11

class CShader;

class ManagerShader {
private:
	static std::map<std::string, CShader*> m_ShaderList;
	static const std::string m_ShaderName[SHADER_MAX];

	static void LoadVertexShader(std::string);
	static void LoadPixelShader(std::string);
	static void LoadGeometryShader(std::string);
	static void LoadInputLayout(std::string, ID3D10Blob*);
	static void Load(std::string name);
	static DXGI_FORMAT GetDxgiFormat(D3D10_REGISTER_COMPONENT_TYPE, BYTE);
public:

	static bool CompileShader();
	static CShader* GetShader(std::string name);
};