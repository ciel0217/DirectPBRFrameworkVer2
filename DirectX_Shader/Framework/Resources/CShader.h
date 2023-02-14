#pragma once
#include <string>
#include "../Resources/DevelopStruct.h"


class CShader {
private:
	VertexShaderRelated* m_ShaderVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ShaderPS;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_ShaderGS;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ShaderCS;

public:
	CShader() { m_ShaderVS = new VertexShaderRelated(); };
	~CShader() { delete m_ShaderVS; }
	//bool SetupShader(D3D11_INPUT_ELEMENT_DESC *layout, UINT num);

	VertexShaderRelated* GetShaderVS() { return m_ShaderVS;}
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetShaderPS() { return m_ShaderPS; }
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> GetShaderGS() { return m_ShaderGS; }
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> GetShaderCS() { return m_ShaderCS; }

	void SetShaderVS(ID3D11VertexShader* shader) { m_ShaderVS->VertexShader.Attach(shader); }
	void SetRFVS(ID3D11ShaderReflection* rf) { m_ShaderVS->ShaderRF.Attach(rf); }
	void SetInputLayout(ID3D11InputLayout* layout) { m_ShaderVS->Layout.Attach(layout); }

	void SetShaderPS(ID3D11PixelShader* shader) { m_ShaderPS.Attach(shader); }
	void SetShaderGS(ID3D11GeometryShader* shader) { m_ShaderGS.Attach(shader); }
	void SetShaderCS(ID3D11ComputeShader* shader) { m_ShaderCS.Attach(shader); }


};