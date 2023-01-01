#pragma once
#include "CRenderer.h"
#include "../Resources/DevelopEnum.h"
#include "../Resources/StructuredBuffer.h"

class CMaterial;

class ParticleRenderer : public CRenderer
{
protected:
	CMaterial* m_CMaterial;
	ParticleRenderType m_RendereType;
	std::unique_ptr<StructuredBuffer> m_StructuredBuffer;

public:
	ParticleRenderer(){}
	void SetUpMaterial(int render_queue, std::string material_name, std::string shader_name, std::string texture_name, MATERIAL_CBUFFER material_value);
	void CreateStructuredBuffer(UINT MaxNumElements);
	virtual ~ParticleRenderer() {}
	virtual void Draw(unsigned int index)override;
};