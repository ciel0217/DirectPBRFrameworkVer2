#include "SpriteRenderer.h"
#include "../Manager/ManagerTexture.h"
#include "../Resources/SquareSprite.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/DevelopEnum.h"
#include "../Resources/CShader.h"
#include "../Resources/CBuffer.h"
#include "../Manager/ManagerMaterial.h"
#include "../Manager/ManagerShader.h"
#include "../Resources/CGameObject.h"

SpriteRenderer::~SpriteRenderer()
{
	m_VertexBuffer.clear();
	m_VertexBuffer.shrink_to_fit();

	m_IndexBuffer.clear();
	m_IndexBuffer.shrink_to_fit();
}

void SpriteRenderer::SetUpSprite(CGameObject* self)
{
	//TODO : いろいろなSpriteTypeを指定できるようにする
	SpriteType* sprite = new SquareSprite();
	std::vector<VERTEX_3D> vertices = sprite->GetVerticies();
	std::vector<UINT> indices = sprite->GetIndices();
	
	m_IndexSize = indices.size();
	m_VertexBuffer.resize(1);
	m_IndexBuffer.resize(1);

	m_VertexBuffer[0].Attach(CDxRenderer::GetRenderer()->CreateVertexBuffer(vertices.size(), vertices.data()));
	m_IndexBuffer[0].Attach(CDxRenderer::GetRenderer()->CreateIndexBuffer(m_IndexSize, indices.data()));


	m_Self = self;
}

void SpriteRenderer::SetUpMaterial(std::string texture_name, std::string shader_name, std::string material_name)
{
	m_TextureName = texture_name;
	ID3D11ShaderResourceView* texture = ManagerTexture::LoadTexture(m_TextureName);
	CShader* shader = ManagerShader::GetShader(shader_name);
	
	unsigned int material_id = ManagerMaterial::CreateMaterial(e2D, shader, texture, material_name);
	m_MaterialIds.reserve(1);
	m_MaterialIds.push_back(material_id);
}

void SpriteRenderer::SetUpMaterial(ID3D11ShaderResourceView * texture, std::string shader_name, std::string material_name)
{
	CShader* shader = ManagerShader::GetShader(shader_name);

	unsigned int material_id = ManagerMaterial::CreateMaterial(e2D, shader, texture, material_name);
	m_MaterialIds.reserve(1);
	m_MaterialIds.push_back(material_id);
}

void SpriteRenderer::Draw(unsigned int index)
{
	if (m_IsActive) {
		D3DXVECTOR3 position = m_Self->GetPosition();
		D3DXQUATERNION rot = m_Self->GetRotation();
		D3DXVECTOR3 scale = m_Self->GetScale()/2.0f;
		std::shared_ptr<CMaterial> renderer_material = ManagerMaterial::GetMaterial(m_MaterialIds[index]);

		MATERIAL_CBUFFER material = renderer_material->GetMaterialValue();
		material.BaseColor = m_TextureColor;


		CShader* shader = renderer_material->GetShader();

		CDxRenderer::GetRenderer()->SetVertexShader(shader->GetShaderVS()->VertexShader.Get());
		CDxRenderer::GetRenderer()->SetPixelShader(shader->GetShaderPS().Get());
		CDxRenderer::GetRenderer()->SetInputLayout(shader->GetShaderVS()->Layout.Get());
		
		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		CDxRenderer::GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer[0].GetAddressOf(), &stride, &offset);
		// インデックスバッファ設定
		CDxRenderer::GetRenderer()->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[0].Get(), DXGI_FORMAT_R32_UINT, 0);

		// プリミティブトポロジ設定
		CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11ShaderResourceView* texture = renderer_material->GetAlbedoTexture().Get();
		if (texture) {
			// テクスチャ設定
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
		}

		D3D11_MAPPED_SUBRESOURCE msr;

		HRESULT ht = CDxRenderer::GetRenderer()->GetDeviceContext()->Map(m_VertexBuffer[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

		D3DXCOLOR color = material.BaseColor;
		D3DXVECTOR2 uv = renderer_material->GetTextureOffset();
		D3DXVECTOR2 uv_wh = renderer_material->GetTextureScale();

		// 頂点０番（左上の頂点）
		vertex[0].Position = position - scale;
		vertex[0].Diffuse = color;
		vertex[0].TexCoord = uv;

		// 頂点１番（右上の頂点）
		vertex[1].Position = D3DXVECTOR3(position.x + scale.x, position.y - scale.y, 0.0f);
		vertex[1].Diffuse = color;
		vertex[1].TexCoord = D3DXVECTOR2(uv.x + uv_wh.x, uv.y);

		// 頂点２番（左下の頂点）
		vertex[2].Position = D3DXVECTOR3(position.x - scale.x, position.y + scale.y, 0.0f);
		vertex[2].Diffuse = color;
		vertex[2].TexCoord = D3DXVECTOR2(uv.x, uv.y + uv_wh.y);

		// 頂点３番（右下の頂点）
		vertex[3].Position = position + scale;
		vertex[3].Diffuse = color;
		vertex[3].TexCoord = uv + uv_wh;
		
		CDxRenderer::GetRenderer()->GetDeviceContext()->Unmap(m_VertexBuffer[0].Get(), 0);

		CDxRenderer::GetRenderer()->SetDepthEnable(false);
		CDxRenderer::GetRenderer()->GetDeviceContext()->DrawIndexed(m_IndexSize, 0, 0);

		CDxRenderer::GetRenderer()->UnbindShaderResourceView(0);
	}
}
