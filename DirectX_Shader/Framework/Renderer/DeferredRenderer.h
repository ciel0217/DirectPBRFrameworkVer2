#pragma once
#include "struct.h"


class CBuffer;
class CShader;

class DeferredRenderer {
private:
	CShader* m_Shader;
	CBuffer* m_DeferredCBuffer;

	DEFERRED_CBUFFER m_CBufferValue;
	ID3D11ShaderResourceView* m_IrradianceTexture;
	ID3D11ShaderResourceView* m_PrefilterTexture;
	ID3D11ShaderResourceView* m_BrdfLUTexture;

public:
	DeferredRenderer() {};
	~DeferredRenderer();

	void Config();
	void Init();
	void Uninit();
	void Draw();

	void UseEnvMap(ID3D11ShaderResourceView* irradiance, ID3D11ShaderResourceView* prefileter, ID3D11ShaderResourceView* brdflut);
};