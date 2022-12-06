#include "MeshRenderer.h"
#include "../Resources/Mesh.h"
#include "../Manager/ManagerModel.h"
#include "../Resources/CBuffer.h"
#include "../Manager/ManagerMaterial.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/CShader.h"
#include "../Resources/DevelopEnum.h"
#include "../Resources/CGameObject.h"

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
		for (auto index : indies) {
			ID3D11Buffer* buffer = CDxRenderer::GetRenderer()->CreateIndexBuffer(index.size(), index.data());
			m_IndexBuffer.push_back(buffer);
		}
	}
}

void MeshRenderer::SetMeshAndMaterial(Mesh * mesh, std::vector<unsigned int> material_ids)
{
	m_ModelMesh.reset(mesh);
	m_MaterialIds.clear();
	m_MaterialIds = material_ids;
	m_MaterialIds.shrink_to_fit();

	CalcBuffer();
}

void MeshRenderer::SetUpModel(std::string model_name, CGameObject * self)
{
	m_Self = self;

	std::pair<std::shared_ptr<Mesh>, std::vector<unsigned int>> model = ManagerModel::Load(model_name);
	m_MaterialIds = model.second;
	m_ModelMesh = model.first;

	CalcBuffer();
}

void MeshRenderer::AddMaterial(unsigned int material_id)
{
	m_MaterialIds.push_back(material_id);
}

void MeshRenderer::DeleteMaterial(unsigned int index)
{
	m_MaterialIds.erase(m_MaterialIds.begin() + index);
}

bool MeshRenderer::ChangeMaterial(unsigned int index, unsigned int change_material_id)
{
	if (m_MaterialIds.size() - 1 < index)return false;
	m_MaterialIds[index] = change_material_id;
	return true;
}

void MeshRenderer::SetUpMaterial(std::string texture_name, std::string shader_name, std::string material_name)
{
}

void MeshRenderer::Draw(unsigned int index)
{

	if (m_IsActive) {
		unsigned int material_id = m_MaterialIds[index];
		std::shared_ptr<CMaterial> material = ManagerMaterial::GetMaterial(material_id);
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

		m_WorldMatrixCBuffer->UpdateBuffer(&mtxWorld);
		m_WorldMatrixCBuffer->VSSetCBuffer(0);

		m_InverseWorldMatrixCBuffer->UpdateBuffer(&mtxInverseWorld);
		m_InverseWorldMatrixCBuffer->VSSetCBuffer(6);

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
		if (material->GetMaterialValue().UseAlbedoTex == 0)
		{
			int a = 0;
			a = 1;
		}
			

		CDxRenderer::GetRenderer()->GetDeviceContext()->DrawIndexed(indices.size(), 0, 0);
		//カリングを元に戻す
		CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_BACK);

		CDxRenderer::GetRenderer()->UnbindShaderResourceView(0);
	}

}
