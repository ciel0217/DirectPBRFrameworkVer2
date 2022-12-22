#pragma once
#include "CRenderer.h"
#include "../Resources/DevelopEnum.h"


class CMaterial;

class ParticleRenderer : public CRenderer
{
protected:
	CMaterial* m_CMaterial;
	ParticleRenderType m_RendereType;

public:
	ParticleRenderer(){}
	void SetUpMaterial(int render_queue, std::string material_name, std::string shader_name, ID3D11ShaderResourceView* texture, MATERIAL_CBUFFER material_value);
	virtual ~ParticleRenderer() {}
	virtual void Draw(unsigned int index)override;
};