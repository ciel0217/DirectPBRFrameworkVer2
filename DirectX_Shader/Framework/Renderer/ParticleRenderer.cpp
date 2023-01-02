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


void ParticleRenderer::SetUpMaterial(int render_queue, std::string material_name, std::string shader_name, std::string texture_name, MATERIAL_CBUFFER material_value)
{
	CShader* shader = ManagerShader::GetShader(shader_name);
	ID3D11ShaderResourceView* texture = ManagerTexture::LoadTexture(texture_name);
	unsigned int id = ManagerMaterial::CreateMaterial(render_queue, shader, texture, material_name);
	m_MaterialIds.push_back(id);

	m_CMaterial = new CMaterial();
	memcpy(m_CMaterial, ManagerMaterial::GetMaterial(id).get(), sizeof(CMaterial));
	m_CMaterial->SetMaterialValue(material_value);

	VERTEX_3D *vertex = new VERTEX_3D;
	D3D11_BUFFER_DESC dsc{};
	dsc.Usage = D3D11_USAGE_DEFAULT;
	dsc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dsc.ByteWidth = sizeof(VERTEX_3D) * 4;
	dsc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ID3D11Buffer * buffer = CDxRenderer::GetRenderer()->CreateVertexBuffer(4, nullptr);
	m_VertexBuffer.push_back(buffer);

}

void ParticleRenderer::CreateStructuredBuffer(UINT MaxNumElements)
{	

	m_StructuredBuffer.reset(StructuredBuffer::CreateStructuredBuffer(sizeof(ParticleStructuredBuffer), MaxNumElements));
}

void ParticleRenderer::Draw(unsigned int index)
{
	//このやり方よくないのかな...
	std::vector<CParticle*> particle_list = ((ParticleSystem*)this)->GetParticleList();
	CShader* shader = m_CMaterial->GetShader();
	CDxRenderer::GetRenderer()->SetAlphaTestEnable(true);
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

	for (auto particle : particle_list)
	{
		if (particle->GetIsDeath())continue;

		D3DXVECTOR3 pos = particle->GetPosition();
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			CDxRenderer::GetRenderer()->GetDeviceContext()->Map(m_VertexBuffer[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

			D3DXVECTOR2 wh = particle->GetScale();
			
			D3DXVECTOR2 uv = particle->GetUV();
			D3DXVECTOR2 uv_wh = particle->GetOffset();

			float width = wh.x;
			float height = wh.y;

			D3DXVECTOR3 dir = camera->GetPosition() - pos;
			D3DXVec3Normalize(&dir, &dir);

			// 頂点座標の設定
			vertex[2].Position = D3DXVECTOR3(-width / 2.0f, -height / 2.0f, 0.0f);//左下
			vertex[3].Position = D3DXVECTOR3(width / 2.0f, -height / 2.0f, 0.0f);//右下
			vertex[0].Position = D3DXVECTOR3(-width / 2.0f, height / 2.0f, 0.0f);//左上
			vertex[1].Position = D3DXVECTOR3(width / 2.0f, height / 2.0f, 0.0f);//右上

			//法線の設定
			vertex[0].Normal = dir;
			vertex[1].Normal = dir;
			vertex[2].Normal = dir;
			vertex[3].Normal = dir;

			// 頂点カラーの設定
			vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			vertex[0].TexCoord = D3DXVECTOR2(uv.x, uv.y);
			vertex[1].TexCoord = D3DXVECTOR2(uv.x + uv_wh.x, uv.y);
			vertex[2].TexCoord = D3DXVECTOR2(uv.x, uv.y + uv_wh.y);
			vertex[3].TexCoord = D3DXVECTOR2(uv.x + uv_wh.x, uv.y + uv_wh.y);

			CDxRenderer::GetRenderer()->GetDeviceContext()->Unmap(m_VertexBuffer[0].Get(), 0);
		}

		D3DXMATRIX mtx_translate, world;
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&mtx_translate, pos.x, pos.y, pos.z);

		D3DXMatrixMultiply(&world, &mtx_world, &mtx_translate);
		D3DXMatrixTranspose(&world, &world);

		m_WorldMatrixCBuffer->UpdateBuffer(&world);
		m_WorldMatrixCBuffer->VSSetCBuffer(0);

		for (int i = 0; i < 10; i++) {
			D3DXMatrixIdentity(&world);
			D3DXMatrixTranslation(&mtx_translate, pos.x, pos.y + 10.0f * i, pos.z);

			D3DXMatrixMultiply(&world, &mtx_world, &mtx_translate);
			D3DXMatrixTranspose(&world, &world);
			particle_buffer.push_back({ world, particle->GetColor(), particle->GetScale(), particle->GetUV(), particle->GetOffset() });
		}
		//ジオメトリシェーダーにセット
		m_StructuredBuffer->UpdateBuffer(particle_buffer.data());
		m_StructuredBuffer->VSSetStructuredBuffer(0);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		CDxRenderer::GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer[0].GetAddressOf(), &stride, &offset);


		m_MaterialCBuffer->UpdateBuffer(&m_CMaterial->GetMaterialValue());
		m_MaterialCBuffer->PSSetCBuffer(3);

		// プリミティブトポロジ設定
		CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		


		CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_NONE);
		CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, m_CMaterial->GetAlbedoTexture().GetAddressOf());
		CDxRenderer::GetRenderer()->GetDeviceContext()->DrawInstanced(4, 10, 0, 0);

		//カリングを元に戻す
		CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_BACK);
		
	}

	

	CDxRenderer::GetRenderer()->UnbindShaderResourceView(0);
	CDxRenderer::GetRenderer()->SetGeometryShader(nullptr);
}
