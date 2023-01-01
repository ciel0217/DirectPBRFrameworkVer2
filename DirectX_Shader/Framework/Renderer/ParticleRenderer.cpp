#include "ParticleRenderer.h"
#include "../Resources/ParticleSystem.h"
#include "../Manager/ManagerTexture.h"
#include "../Manager/ManagerMaterial.h"
#include "../Manager/ManagerShader.h"
#include "../Manager/ManagerScene.h"
#include "../Resources/CParticle.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/StructuredBuffer.h"


void ParticleRenderer::SetUpMaterial(int render_queue, std::string material_name, std::string shader_name, std::string texture_name, MATERIAL_CBUFFER material_value)
{
	CShader* shader = ManagerShader::GetShader(shader_name);
	ID3D11ShaderResourceView* texture = ManagerTexture::LoadTexture(texture_name);
	unsigned int id = ManagerMaterial::CreateMaterial(render_queue, shader, texture, material_name);
	m_MaterialIds.push_back(id);

	m_CMaterial = new CMaterial();
	memcpy(m_CMaterial, ManagerMaterial::GetMaterial(id).get(), sizeof(CMaterial));
	m_CMaterial->SetMaterialValue(material_value);

}

void ParticleRenderer::CreateStructuredBuffer(UINT MaxNumElements)
{
	m_StructuredBuffer.reset(StructuredBuffer::CreateStructuredBuffer(sizeof(ParticleStructuredBuffer), MaxNumElements));
}

void ParticleRenderer::Draw(unsigned int index)
{
	//このやり方よくないのかな...
	std::vector<CParticle*> particle_list = ((ParticleSystem*)this)->GetParticleList();

	Camera* camera = ManagerScene::GetInstance()->GetCurrentSceneCamera();

	D3DXMATRIX mtx_world;
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx_world);

	D3DXMATRIX mtx_view, mtx_inverse_view;
	D3DXMatrixLookAtLH(&mtx_view, &camera->GetPosition(), &camera->GetCameraLookAtPoint(), &camera->GetCameraUp());

	mtx_world._11 = mtx_view._11;
	mtx_world._12 = mtx_view._21;
	mtx_world._13 = mtx_view._31;
	mtx_world._21 = mtx_view._12;
	mtx_world._22 = mtx_view._22;
	mtx_world._23 = mtx_view._32;
	mtx_world._31 = mtx_view._13;
	mtx_world._32 = mtx_view._23;
	mtx_world._33 = mtx_view._33;

	std::vector<ParticleStructuredBuffer> particle_buffer;
	for (auto particle : particle_list)
	{
		if (particle->GetIsDeath())continue;

		D3DXVECTOR3 pos = particle->GetPosition();

		D3DXMATRIX mtx_translate, world;
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&mtx_translate, pos.x, pos.y, pos.z);

		D3DXMatrixMultiply(&world, &mtx_world, &mtx_translate);
		D3DXMatrixTranspose(&world, &world);
		particle_buffer.push_back({world, particle->GetColor(), particle->GetScale(), particle->GetUV(), particle->GetOffset()});
	}
	
	//ジオメトリシェーダーにセット
	m_StructuredBuffer->UpdateBuffer(particle_buffer.data());
	m_StructuredBuffer->GSSetStructuredBuffer(0);

	m_MaterialCBuffer->UpdateBuffer(&m_CMaterial->GetMaterialValue());
	m_MaterialCBuffer->PSSetCBuffer(3);

	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_NONE);

	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_CMaterial->GetAlbedoTexture());
	CDxRenderer::GetRenderer()->GetDeviceContext()->DrawInstanced(4, particle_buffer.size(), 0, 0);

	//カリングを元に戻す
	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_BACK);

	CDxRenderer::GetRenderer()->UnbindShaderResourceView(0);
}
