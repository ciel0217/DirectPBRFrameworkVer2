#include "ParticleRenderer.h"
#include "../Resources/ParticleSystem.h"
#include "../Manager/ManagerTexture.h"
#include "../Manager/ManagerMaterial.h"
#include "../Manager/ManagerShader.h"

void ParticleRenderer::SetUpMaterial(int render_queue, std::string material_name, std::string shader_name, ID3D11ShaderResourceView * texture, MATERIAL_CBUFFER material_value)
{
	CShader* shader = ManagerShader::GetShader(shader_name);
	unsigned int id = ManagerMaterial::CreateMaterial(render_queue, shader, texture, material_name);

	m_CMaterial = new CMaterial();
	memcpy(m_CMaterial, ManagerMaterial::GetMaterial(id).get(), sizeof(CMaterial));
	m_CMaterial->SetMaterialValue(material_value);

}

void ParticleRenderer::Draw(unsigned int index)
{
	//‚±‚Ì‚â‚è•û‚æ‚­‚È‚¢‚Ì‚©‚È...
	std::vector<CParticle*> particle_list = ((ParticleSystem*)this)->GetParticleList();
}
