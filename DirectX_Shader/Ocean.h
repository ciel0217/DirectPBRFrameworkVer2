#pragma once
#include "../Resources/CGameObject.h"
#include "struct.h"
#include "MeshRenderer.h"

class CBuffer;

class Ocean : public CGameObject, public MeshRenderer
{
private:
	OCEAN_CBUFFER m_CBufferValue;
	CBuffer* m_CBuffer;
	std::string m_NormalMapTextureName;
	ID3D11ShaderResourceView* m_NormalTexture;

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};