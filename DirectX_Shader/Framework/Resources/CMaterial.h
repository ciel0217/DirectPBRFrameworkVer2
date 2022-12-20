#pragma once
#include "../LowLevel/DirectX.h"
#include "../Resources/DevelopStruct.h"
#include "../Resources/DevelopEnum.h"
#include <string>

class CShader;
class CMaterial {
private:
	std::string m_MaterialName;
	unsigned int m_MaterialId;
	MATERIAL_CBUFFER m_MaterialValue;
	int m_RenderQueue;
	CShader* m_Shader;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_AlbedoTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NormalTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_RoughMetalTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_AoTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_EmissiveTexture;

	D3DXVECTOR2 m_TextureOffset = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_TextureScale = D3DXVECTOR2(1.0f, 1.0f);
	
public:
	CMaterial() {};
	CMaterial(int render, CShader* shader, ID3D11ShaderResourceView* texture, std::string name) : 
		m_RenderQueue(render), m_Shader(shader), m_MaterialName(name) {
		ZeroMemory(&m_MaterialValue, sizeof(m_MaterialValue));
		m_MaterialValue.BaseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_AlbedoTexture.Attach(texture);
	}

	CMaterial(int render, MATERIAL_CBUFFER material, CShader* shader, ID3D11ShaderResourceView* texture, std::string name) :
		m_RenderQueue(render), m_Shader(shader), m_MaterialName(name), m_MaterialValue(material) {
		m_AlbedoTexture.Attach(texture);
	}

	void SetMaterialValue(MATERIAL_CBUFFER value) { m_MaterialValue = value; }
	MATERIAL_CBUFFER GetMaterialValue() { return m_MaterialValue; }
	
	unsigned int GetMaterialId() { return m_MaterialId; }
	void SetMateriaId(unsigned int id) { m_MaterialId = id; }
	
	int GetRenderQueue() { return m_RenderQueue; }
	void SetRenderQueue(int queue) { m_RenderQueue = queue; }

	void SetAlbedoTexture(ID3D11ShaderResourceView* texture) { m_AlbedoTexture.Attach(texture); }
	void SetNormalTexture(ID3D11ShaderResourceView* texture) { m_NormalTexture.Attach(texture); }
	void SetRoughMetalTexture(ID3D11ShaderResourceView* texture) { m_RoughMetalTexture.Attach(texture); }
	void SetAoTexture(ID3D11ShaderResourceView* texture) { m_AoTexture.Attach(texture); }
	void SetEmissiveTexture(ID3D11ShaderResourceView* texture) { m_EmissiveTexture.Attach(texture); }
	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetAlbedoTexture() { return m_AlbedoTexture; }
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetNormalTexture() { return m_NormalTexture; }
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetRoughMetalTexture() { return m_RoughMetalTexture; }
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetAoTexture() { return m_AoTexture; }
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetEmissiveTexture() { return m_EmissiveTexture; }

	CShader* GetShader() { return m_Shader; }
	void SetShader(std::string shader_name);
	
	D3DXVECTOR2 GetTextureOffset() { return m_TextureOffset; }
	D3DXVECTOR2 GetTextureScale() { return m_TextureScale; }
	D3DXVECTOR2 SetTextureOffset(D3DXVECTOR2 offset) { m_TextureOffset = offset; }
	D3DXVECTOR2 SetTextureScale(D3DXVECTOR2 scale) { m_TextureScale = scale; }


};