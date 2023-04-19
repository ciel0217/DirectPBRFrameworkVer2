#include "ParticleRenderer.h"
#include "../Resources/ParticleSystem.h"
#include "../Manager/ManagerTexture.h"
#include "../Manager/ManagerMaterial.h"
#include "../Manager/ManagerShader.h"
#include "../Manager/ManagerScene.h"
#include "../Resources/CParticle.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/StructuredBuffer.h"
#include "../Resources/CShader.h"
#include <algorithm>


void ParticleRenderer::SetUpMaterial(int render_queue, std::string material_name, std::string shader_name, std::string texture_name, MATERIAL_CBUFFER material_value)
{
	CShader* shader = ManagerShader::GetShader(shader_name);
	ID3D11ShaderResourceView* texture = ManagerTexture::LoadTexture(texture_name);
	unsigned int id = ManagerMaterial::CreateMaterial(render_queue, shader, texture, material_name);

	CMaterial *a = new CMaterial();
	ManagerMaterial::GetMaterial(id, a);
	m_Materials.push_back(a);

	m_Materials[0]->SetAllMaterialValue(material_value);

}

void ParticleRenderer::CreateStructuredBuffer(UINT MaxNumElements)
{	

	m_StructuredBuffer.reset(StructuredBuffer::CreateStructuredBuffer(sizeof(ParticleStructuredBuffer), MaxNumElements));
}

D3DXVECTOR3 ParticleRenderer::GetBounds()
{
	std::vector<CParticle*> particle_list = ((ParticleSystem*)this)->GetParticleList();

	auto x_bounds = std::minmax_element(particle_list.begin(), particle_list.end(),
		[](CParticle* a, CParticle* b) 
		{ 
			if (a->GetIsDeath() || b->GetIsDeath())
				return false;
				
			return a->GetPosition().x < b->GetPosition().x;
		});

	auto y_bounds = std::minmax_element(particle_list.begin(), particle_list.end(),
		[](CParticle* a, CParticle* b)
		{
			if (a->GetIsDeath() || b->GetIsDeath())
				return false;

			return a->GetPosition().y < b->GetPosition().y;
		});

	auto z_bounds = std::minmax_element(particle_list.begin(), particle_list.end(),
		[](CParticle* a, CParticle* b)
		{
			if (a->GetIsDeath() || b->GetIsDeath())
				return false;

			return a->GetPosition().z < b->GetPosition().z;
		});


	D3DXVECTOR3 min = D3DXVECTOR3
	(
		(*x_bounds.first)->GetPosition().x - (*x_bounds.first)->GetScale().x / 2.0f, 
		(*y_bounds.first)->GetPosition().y - (*y_bounds.first)->GetScale().y / 2.0f,
		(*z_bounds.first)->GetPosition().z - (*z_bounds.first)->GetScale().z / 2.0f
	);

	D3DXVECTOR3 max = D3DXVECTOR3
	(
		(*x_bounds.second)->GetPosition().x + (*x_bounds.second)->GetScale().x / 2.0f,
		(*y_bounds.second)->GetPosition().y + (*y_bounds.second)->GetScale().y / 2.0f,
		(*z_bounds.second)->GetPosition().z + (*z_bounds.second)->GetScale().z / 2.0f
	);

	return max-min;
}

void ParticleRenderer::Draw(unsigned int index)
{
	//このやり方よくないのかな...
	std::vector<CParticle*> particle_list = ((ParticleSystem*)this)->GetParticleList();
	CShader* shader = m_Materials[0]->GetShader();
	
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

	CDxRenderer::GetRenderer()->SetVertexShader(shader->GetShaderVS()->VertexShader.Get());
	CDxRenderer::GetRenderer()->SetPixelShader(shader->GetShaderPS().Get());
	CDxRenderer::GetRenderer()->SetInputLayout(shader->GetShaderVS()->Layout.Get());
	CDxRenderer::GetRenderer()->SetGeometryShader(shader->GetShaderGS().Get());

	int count = 0;
	for (auto particle : particle_list)
	{
		if (particle->GetIsDeath())
		{
			count++;
			continue;
		}

		D3DXVECTOR3 pos = particle->GetPosition();

		D3DXMATRIX mtx_translate, world;
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&mtx_translate, pos.x, pos.y, pos.z);

		D3DXMatrixMultiply(&world, &mtx_world, &mtx_translate);
		D3DXMatrixTranspose(&world, &world);

		/*m_WorldMatrixCBuffer->UpdateBuffer(&world);
		m_WorldMatrixCBuffer->GSSetCBuffer(0);*/

		particle_buffer.push_back({ world, particle->GetColor(), particle->GetScale(), particle->GetUV(), particle->GetOffset() });

	}

	particle_buffer.shrink_to_fit();
	//ジオメトリシェーダーにセット
	m_StructuredBuffer->UpdateBuffer(particle_buffer.data(), particle_buffer.size());
	m_StructuredBuffer->GSSetStructuredBuffer(2);

	m_MaterialCBuffer->UpdateBuffer(&m_Materials[0]->GetMaterialValue());
	m_MaterialCBuffer->PSSetCBuffer(3);

	// プリミティブトポロジ設定
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_NONE);
	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, m_Materials[0]->GetAlbedoTexture().GetAddressOf());
	CDxRenderer::GetRenderer()->GetDeviceContext()->DrawInstanced(4, particle_buffer.size(), 0, 0);

	//カリングを元に戻す
	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_BACK);

	CDxRenderer::GetRenderer()->UnbindShaderResourceView(0);
	CDxRenderer::GetRenderer()->SetGeometryShader(nullptr);
}
