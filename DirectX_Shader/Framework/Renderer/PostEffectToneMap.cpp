#include "PostEffectToneMap.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Manager/ManagerShader.h"
#include "../Manager/ManagerScene.h"
#include "CShader.h"
#include "SkyBox.h"


void PostEffectToneMap::Config()
{
	m_Shader = ManagerShader::GetShader("ToneMap.hlsl");
}

void PostEffectToneMap::Init()
{

}

void PostEffectToneMap::Uninit()
{
}

void PostEffectToneMap::Draw()
{
	CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
	CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
	CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	RenderTarget tonemap = CDxRenderer::GetRenderer()->GetSceneTexture(eToneMap);
	//ID3D11ShaderResourceView* a = CDxRenderer::GetRenderer()->GetDepthStencilSRV();
	ID3D11ShaderResourceView* t0 = tonemap.ShaderResourceView.Get();

	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, &t0);

	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_NONE);
	CDxRenderer::GetRenderer()->GetDeviceContext()->Draw(4, 0);
	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_BACK);
	CDxRenderer::GetRenderer()->UnbindShaderResourceView(0);


}
