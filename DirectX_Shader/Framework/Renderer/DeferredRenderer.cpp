#include "DeferredRenderer.h"
#include "../Manager/ManagerShader.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/CShader.h"
#include "../Manager/ManagerScene.h"
#include "../Resources/CommonProcess.h"
#include "../Resources/CBuffer.h"
#include "../Resources/SkyBox.h"


DeferredRenderer::~DeferredRenderer()
{
	delete m_DeferredCBuffer;
}

void DeferredRenderer::Config()
{
	ZeroMemory(&m_CBufferValue, sizeof(m_CBufferValue));
	m_Shader = ManagerShader::GetShader("Shader/Deferred.hlsl");
	if (!m_DeferredCBuffer) {
		m_DeferredCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(DEFERRED_CBUFFER), D3D11_BIND_CONSTANT_BUFFER, nullptr));
	}
}

void DeferredRenderer::Init()
{
	
}

void DeferredRenderer::Uninit()
{
}

void DeferredRenderer::Draw()
{
	
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	//CDxRenderer::GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//CDxRenderer::GetRenderer()->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);


	CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
	CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
	CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());

	m_DeferredCBuffer->UpdateBuffer(&m_CBufferValue);
	m_DeferredCBuffer->PSSetCBuffer(5);

	RenderTarget color = CDxRenderer::GetRenderer()->GetSceneTexture(eBaseColor);
	RenderTarget normal = CDxRenderer::GetRenderer()->GetSceneTexture(eNormal);
	RenderTarget rough_meta_spe = CDxRenderer::GetRenderer()->GetSceneTexture(eRoughnessAndMeralicAndSpecular);
	RenderTarget emmision = CDxRenderer::GetRenderer()->GetSceneTexture(eEmmision);


	ID3D11ShaderResourceView* t0 = color.ShaderResourceView.Get();
	ID3D11ShaderResourceView* t1 = normal.ShaderResourceView.Get();
	ID3D11ShaderResourceView* t2 = rough_meta_spe.ShaderResourceView.Get();
	ID3D11ShaderResourceView* t3 = emmision.ShaderResourceView.Get();

	ID3D11ShaderResourceView* t4 = CDxRenderer::GetRenderer()->GetDepthStencilSRV();


	//ID3D11ShaderResourceView* te = ManagerTexture::GetTexture("a.png");

	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, &t0);
	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(1, 1, &t1);
	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(2, 1, &t2);
	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(3, 1, &t3);

	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(4, 1, &t4);

	/*ID3D11ShaderResourceView* t8 = m_SkyBox->GetEnvSkyMap();
	ID3D11ShaderResourceView* t9 = m_SkyBox->GetIrradianceMap();
	ID3D11ShaderResourceView* t10 = m_SkyBox->GetSpecularMap();
	ID3D11ShaderResourceView* t11 = m_SkyBox->GetBrdfLUTMap();*/

	if (m_CBufferValue.UseEnvMap == 1)
	{
		/*CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(8, 1, &t8);*/
		CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(9, 1, &m_IrradianceTexture);
		CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(10, 1, &m_PrefilterTexture);
		CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(11, 1, &m_BrdfLUTexture);
	}

	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_NONE);
	CDxRenderer::GetRenderer()->GetDeviceContext()->Draw(4, 0);
	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_BACK);

	CDxRenderer::GetRenderer()->UnbindShaderResourceView(0);
	CDxRenderer::GetRenderer()->UnbindShaderResourceView(1);
	CDxRenderer::GetRenderer()->UnbindShaderResourceView(2);
	CDxRenderer::GetRenderer()->UnbindShaderResourceView(3);
	CDxRenderer::GetRenderer()->UnbindShaderResourceView(4);
	CDxRenderer::GetRenderer()->UnbindShaderResourceView(8);
	CDxRenderer::GetRenderer()->UnbindShaderResourceView(9);
	CDxRenderer::GetRenderer()->UnbindShaderResourceView(10);
	CDxRenderer::GetRenderer()->UnbindShaderResourceView(11);
}

void DeferredRenderer::UseEnvMap(ID3D11ShaderResourceView * irradiance, ID3D11ShaderResourceView * prefileter, ID3D11ShaderResourceView * brdflut)
{
	m_IrradianceTexture = irradiance;
	m_PrefilterTexture = prefileter;
	m_BrdfLUTexture = brdflut;
	m_CBufferValue.UseEnvMap = 1;
}

//void DeferredRenderer::UseEnvMap(SkyBox* obj)
//{
//	m_SkyBox = obj;
//	m_SkyBox->Config();
//	m_SkyBox->Init();
//	m_SkyBox->DrawByCubeMap();
//}
