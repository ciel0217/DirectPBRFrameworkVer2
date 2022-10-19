#pragma once
#include <string>
#include "struct.h"


class CShader {
private:
	VertexShaderRelated* m_ShaderVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ShaderPS;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_ShaderGS;

public:
	CShader() { m_ShaderVS = new VertexShaderRelated(); };
	~CShader() { delete m_ShaderVS; }
	//bool SetupShader(D3D11_INPUT_ELEMENT_DESC *layout, UINT num);

	VertexShaderRelated* GetShaderVS() { return m_ShaderVS;}
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetShaderPS() { return m_ShaderPS; }
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> GetShaderGS() { return m_ShaderGS; }
	void SetShaderPS(ID3D11PixelShader* shader) { m_ShaderPS.Attach(shader); }
	void SetShaderGS(ID3D11GeometryShader* shader) { m_ShaderGS.Attach(shader); }


};