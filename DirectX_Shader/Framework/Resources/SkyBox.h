#pragma once
#include "CGameObject.h"
#include "CCubeMap.h"
#include "MeshRenderer.h"

class CBuffer;
class Mesh;

class SkyBox : public CGameObject, public MeshRenderer {
private:
	CShader* m_Shader;
	CShader* m_ShaderCubeMap;
	CShader* m_ShaderIrradianceMap;
	CShader* m_ShaderPreFilterMap;
	CShader* m_ShaderBrdfLUT;

	CCubeMap* m_CubeMap;
	CCubeMap* m_IrradianceMap;
	CCubeMap* m_PreFilterMap;
	DirectXTexture2D* m_BrdfLUTTexture;

	CBuffer* m_RoughnessCBuffer;
	CBuffer* m_WorldCBuffer;
	

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;

	void DrawByCubeMap();
	ID3D11ShaderResourceView* GetEnvSkyMap() { return m_CubeMap->GetShaderResourceView(); }
	ID3D11ShaderResourceView* GetIrradianceMap() { return m_IrradianceMap->GetShaderResourceView(); }
	ID3D11ShaderResourceView* GetSpecularMap() { return m_PreFilterMap->GetShaderResourceView(); }
	ID3D11ShaderResourceView* GetBrdfLUTMap() { return m_BrdfLUTTexture->GetSRV(); }

};