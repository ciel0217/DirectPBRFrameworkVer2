#include "ParticleRenderer.h"
#include "../Resources/ParticleSystem.h"
#include "../Manager/ManagerTexture.h"
#include "../Manager/ManagerMaterial.h"
#include "../Manager/ManagerShader.h"
#include "../Manager/ManagerScene.h"

void ParticleRenderer::SetUpMaterial(int render_queue, std::string material_name, std::string shader_name, ID3D11ShaderResourceView * texture, MATERIAL_CBUFFER material_value)
{
	CShader* shader = ManagerShader::GetShader(shader_name);
	unsigned int id = ManagerMaterial::CreateMaterial(render_queue, shader, texture, material_name);
	m_MaterialIds.push_back(id);

	m_CMaterial = new CMaterial();
	memcpy(m_CMaterial, ManagerMaterial::GetMaterial(id).get(), sizeof(CMaterial));
	m_CMaterial->SetMaterialValue(material_value);

}

void ParticleRenderer::Draw(unsigned int index)
{
	//このやり方よくないのかな...
	std::vector<CParticle*> particle_list = ((ParticleSystem*)this)->GetParticleList();

	Camera* camera = ManagerScene::GetInstance()->GetCurrentSceneCamera();

	D3DXMATRIX  mtx_scale, mtx_translate, mtx_world;
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx_world);

	D3DXMATRIX mtx_view, mtx_inverse_view;
	D3DXMatrixLookAtLH(&mtx_view, &camera->GetPosition(), &camera->GetCameraLookAtPoint(), &camera->GetCameraUp());
}
