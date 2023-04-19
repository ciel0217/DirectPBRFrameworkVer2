#include "MeshRenderer.h"
#include "../Resources/Mesh.h"
#include "../Manager/ManagerModel.h"
#include "../Resources/CBuffer.h"
#include "../Manager/ManagerMaterial.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/CShader.h"
#include "../Resources/DevelopEnum.h"
#include "../Resources/CGameObject.h"
#include "../Resources/CMaterial.h"

void MeshRenderer::CalcBuffer()
{
	
	//頂点バッファ
	{
		std::vector<VERTEX_3D> vertices = m_ModelMesh->GetVertexArray();

		ID3D11Buffer * buffer = CDxRenderer::GetRenderer()->CreateVertexBuffer(vertices.size(), vertices.data());
		m_VertexBuffer.push_back(buffer);
	}

	//インデックスバッファ
	{
		std::vector<std::vector<unsigned int>> indies = m_ModelMesh->GetIndicesArray();
		for (auto index : indies) 
		{
			ID3D11Buffer* buffer = CDxRenderer::GetRenderer()->CreateIndexBuffer(index.size(), index.data());
			m_IndexBuffer.push_back(buffer);
		}
	}
}

void MeshRenderer::SetMeshAndMaterial(Mesh * mesh, std::vector<CMaterial*> materials)
{
	m_ModelMesh.reset(mesh);

	for (int i = 0; i < m_Materials.size(); i++)
		delete m_Materials[i];

	m_Materials.clear();
	m_Materials = materials;
	m_Materials.shrink_to_fit();

	CalcBuffer();
}

void MeshRenderer::SetUpModel(std::string model_name, std::vector<std::string> material_name)
{
	
	std::shared_ptr<Mesh> model = ManagerModel::Load(model_name);
	m_ModelMesh = model;

	for (unsigned int i = 0; i < material_name.size(); i++)
	{
		CMaterial* a = new CMaterial();
		ManagerMaterial::GetMaterial(material_name[i], a);
		m_Materials.push_back(a);
	}

	CalcBuffer();
}

MeshRenderer::~MeshRenderer()
{
	for (unsigned int i = 0; i < m_Materials.size(); i++)
		delete m_Materials[i];
	
}

void MeshRenderer::AddMaterial(CMaterial* material)
{
	m_Materials.push_back(material);
}

void MeshRenderer::DeleteMaterial(unsigned int index)
{
	m_Materials.erase(m_Materials.begin() + index);
}

D3DXVECTOR3 MeshRenderer::GetBounds()
{
	return m_ModelMesh->GetBounds();
}

bool MeshRenderer::ChangeMaterial(unsigned int index, CMaterial* material)
{
	if (m_Materials.size() - 1 < index)return false;
	m_Materials[index] = material;

	return true;
}


void MeshRenderer::Draw(unsigned int index)
{

	if (m_IsActive) {
		CMaterial* material = m_Materials[index];
		std::vector<unsigned int> indices = m_ModelMesh->GetIndexArray(index);

		CShader* shader = material->GetShader();
		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
		D3DXMATRIX mtxWorld, mtxInverseWorld;

		D3DXVECTOR3 position = m_Self->GetPosition();
		D3DXVECTOR3 scale = m_Self->GetScale();
		D3DXQUATERNION qua = m_Self->GetRotation();

		D3DXMatrixIdentity(&mtxWorld);

		//サイズ
		D3DXMatrixScaling(&mtxScl, scale.x, scale.y, scale.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

		//回転
		D3DXMatrixRotationQuaternion(&mtxRot, &qua);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//移動
		D3DXMatrixTranslation(&mtxTranslate, position.x, position.y, position.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		D3DXMatrixInverse(&mtxInverseWorld, nullptr, &mtxWorld);

		D3DXMatrixTranspose(&mtxWorld, &mtxWorld);
		D3DXMatrixTranspose(&mtxInverseWorld, &mtxInverseWorld);

		WORLD_CBUFFER world_cbuffer;
		world_cbuffer.World = mtxWorld;
		world_cbuffer.InverseWorld = mtxInverseWorld;

		m_WorldCBuffer->UpdateBuffer(&world_cbuffer);
		m_WorldCBuffer->VSSetCBuffer(0);
		m_WorldCBuffer->PSSetCBuffer(0);
		m_WorldCBuffer->GSSetCBuffer(0);

		//カリングなし
		CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_NONE);

		CDxRenderer::GetRenderer()->SetVertexShader(shader->GetShaderVS()->VertexShader.Get());
		CDxRenderer::GetRenderer()->SetPixelShader(shader->GetShaderPS().Get());
		CDxRenderer::GetRenderer()->SetInputLayout(shader->GetShaderVS()->Layout.Get());

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		CDxRenderer::GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer[0].GetAddressOf(), &stride, &offset);

		// インデックスバッファ設定
		CDxRenderer::GetRenderer()->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[index].Get(), DXGI_FORMAT_R32_UINT, 0);

		// プリミティブトポロジ設定
		CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_MaterialCBuffer->UpdateBuffer(&material->GetMaterialValue());
		m_MaterialCBuffer->PSSetCBuffer(3);

		if (material->GetMaterialValue().UseAlbedoTex == 1)
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, material->GetAlbedoTexture().GetAddressOf());
		if (material->GetMaterialValue().UseAoMap == 1)
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(1, 1, material->GetAoTexture().GetAddressOf());
		if (material->GetMaterialValue().UseEmmisive == 1)
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(2, 1, material->GetEmissiveTexture().GetAddressOf());
		if (material->GetMaterialValue().UseOccMetalRough == 1)
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(3, 1, material->GetRoughMetalTexture().GetAddressOf());
		if (material->GetMaterialValue().NormalState != NORMAL_UNUSED)
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(4, 1, material->GetNormalTexture().GetAddressOf());
		
			

		CDxRenderer::GetRenderer()->GetDeviceContext()->DrawIndexed(indices.size(), 0, 0);
		//カリングを元に戻す
		CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_BACK);

		CDxRenderer::GetRenderer()->UnbindShaderResourceView(0);
	}

}
