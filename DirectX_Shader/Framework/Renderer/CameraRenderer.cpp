#include "CameraRenderer.h"
#include "../Manager/ManagerLight.h"
#include "PostEffectToneMap.h"
#include "../Resources/CPostEffect.h"
#include "../Resources/CBuffer.h"
#include "GBufferPass.h"
#include "../LowLevel/CDxRenderer.h"
#include "CRenderer.h"
#include "../Manager/ManagerMaterial.h"
#include "../Resources/CLight.h"
#include "../Resources/CommonProcess.h"
#include "../Resources/DevelopStruct.h"
#include "../Resources/StructuredBuffer.h"
#include "../Resources/UnorderedAccessView.h"
#include "../Manager/ManagerShader.h"
#include "../Resources/CShader.h"
#include "../Resources/Camera.h"
#include "../../Math/Quaternion.h"

#include <vector>
#include <memory>

#define VIEW_NEAR_Z (0.3f)
#define VIEW_FAR_Z  (100000.0f)

std::unique_ptr<DeferredRenderer>         CameraRenderer::m_DeferredRenderPass = nullptr;
std::unique_ptr<CPostEffect>             CameraRenderer::m_ToneMapPass = nullptr;
std::unique_ptr<ManagerLight>		      CameraRenderer::m_LightPass = nullptr;
std::unique_ptr<GBufferPass>			  CameraRenderer::m_GBufferPass = nullptr;

std::unique_ptr<StructuredBuffer>			CameraRenderer::m_FrustumStructuredBuffer = nullptr;
std::unique_ptr<CBuffer>					CameraRenderer::m_FrustumCullCameraInfoCBuffer = nullptr;
std::unique_ptr<UnorderedAccessView>		CameraRenderer::m_FrustumCullUAVBuffer = nullptr;
std::shared_ptr<CShader>					CameraRenderer::m_CSShader = nullptr;

std::unique_ptr<CBuffer>					CameraRenderer::m_CameraCBuffer = nullptr;


void CameraRenderer::SetUpCameraRenderer()
{
	if(!m_DeferredRenderPass)
		m_DeferredRenderPass.reset(new DeferredRenderer());
	if(!m_LightPass)
		m_LightPass.reset(new ManagerLight());
	if(!m_ToneMapPass)
		m_ToneMapPass.reset(new PostEffectToneMap());
	if (!m_GBufferPass)
		m_GBufferPass.reset(new GBufferPass());

	if (!m_FrustumStructuredBuffer)
		m_FrustumStructuredBuffer.reset(StructuredBuffer::CreateStructuredBuffer(sizeof(FrustumCullStructuredBuffer), MAX_CULLING_OBJECT));

	if (!m_FrustumCullCameraInfoCBuffer)
		m_FrustumCullCameraInfoCBuffer.reset(new CBuffer(CBuffer::CreateBuffer(sizeof(FrustumCullCameraCBuffer), D3D11_BIND_CONSTANT_BUFFER, nullptr)));

	if (!m_FrustumCullUAVBuffer)
		m_FrustumCullUAVBuffer.reset(UnorderedAccessView::CreateUnorderedAccessView
		(
			m_FrustumStructuredBuffer->GetStructuredBuffer(),
			MAX_CULLING_OBJECT
		));

	if (!m_CSShader)
		m_CSShader.reset(ManagerShader::GetShader("Shader/FrustumCull.hlsl"));

	if (!m_CameraCBuffer)
		m_CameraCBuffer.reset(new CBuffer(CBuffer::CreateBuffer(sizeof(CAMERA_CBUFFER), D3D11_BIND_CONSTANT_BUFFER, nullptr)));



	return;
}

void CameraRenderer::Release()
{

}

CameraRenderer::CameraRenderer()
{
	m_DeferredRenderPass->Config();
	m_LightPass->Config();
	m_ToneMapPass->Config();
}

void CameraRenderer::SetUpRenderer()
{
	
	for (auto effect : m_PostEffectToOpacity) {
		effect->Config();
	}

	for (auto effect : m_PostEffectToAll) {
		effect->Config();
	}
}

void CameraRenderer::Uninit()
{
	//staticじゃないやつを解放
	for (auto obj : m_PostEffectToOpacity) {
		obj->Uninit();
	}

	for (auto obj : m_PostEffectToAll) {
		obj->Uninit();
	}
	m_DeferredRenderPass->Uninit();
	m_LightPass->Uninit();
	m_ToneMapPass->Uninit();

	m_PostEffectToOpacity.remove_if([](CPostEffect* obj) {
		return obj->Release();
		});
	m_PostEffectToOpacity.clear();

	m_PostEffectToAll.remove_if([](CPostEffect* obj) {
		return obj->Release();
		});
	m_PostEffectToAll.clear();

	delete m_SkyBox;
}

void CameraRenderer::DrawRenderer(std::list<CGameObject *> gameobject[])
{

	CalcRenderingOrder(gameobject);

	CDxRenderer::GetRenderer()->SetDepthEnable(true);

	CDxRenderer::GetRenderer()->SetViewPortDefault();
	CDxRenderer::GetRenderer()->ClearRenderTextureSceneByDeffard();
	CDxRenderer::GetRenderer()->SetRenderTargetByDeffard();

	DrawGBuffer();
	
	CDxRenderer::GetRenderer()->ClearIntermediateBuffer(false);
	CDxRenderer::GetRenderer()->SetRenderTargetIntermediateBuffer(false);

	m_LightPass->SetLightCBuffer();
	m_DeferredRenderPass->Draw();
	
	DrawPostEffectToOpacity();

	CDxRenderer::GetRenderer()->SetRenderTargetIntermediateBuffer(true);
	DrawTransparent();

	CDxRenderer::GetRenderer()->ClearBackBuffor(false);
	CDxRenderer::GetRenderer()->SetRenderTargetBackBuffor(false);

	m_ToneMapPass->Draw();
	
	Draw2D();
	DrawPostEffectToAll();

}

void CameraRenderer::CalcRenderingOrder(std::list<CGameObject *> gameobject[])
{
	ClearGameObjectList();

	for (int i = 0; i < eMaxLayer; i++) {
		for (auto obj : gameobject[i]) {
		
			if (dynamic_cast<CLight*>(obj)) {
				m_LightPass->SetLight(dynamic_cast<CLight*>(obj));
				continue;
			}

			CRenderer* object = dynamic_cast<CRenderer*>(obj);
			if (object) {
				std::vector<unsigned int> material_ids = object->GetMaterialIds();
				unsigned int material_count = 0;
				for (auto id : material_ids) {
					std::shared_ptr<CMaterial> material = ManagerMaterial::GetMaterial(id);
					if (material->GetRenderQueue() <= DrawObjectRenderQueue::eOpacity) {
						m_OpacityList.push_back(std::make_tuple(object, material_count, material));
					}
					else if (material->GetRenderQueue() == DrawObjectRenderQueue::e2D) {
						m_SpriteList.push_back(std::make_tuple(object, material_count, material));
					}
					else {
						m_TransparentList.push_back(std::make_tuple(object, material_count, material));
					}
					material_count++;
				}
			}
		}
	}
	D3DXVECTOR3 pos = m_CameraInfoValue.CameraPos;

	//Sortの優先順位( 1 : renderqueue   2 : カメラとの距離(遠いほうが先に描画))
	m_TransparentList.sort([pos](std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> a, std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> b)
	{
			int a_render = std::get<2>(a)->GetRenderQueue();
			int b_render = std::get<2>(b)->GetRenderQueue();

			if (a_render != b_render)
				return a_render < b_render;
			else
			{
				D3DXVECTOR3 a_dis = pos - ((CommonProcess*)std::get<0>(a))->GetPosition();
				D3DXVECTOR3 b_dis = pos - ((CommonProcess*)std::get<0>(b))->GetPosition();

				return D3DXVec3LengthSq(&a_dis) > D3DXVec3LengthSq(&b_dis);
			}
			
	});

	//近いほうから描画
	m_OpacityList.sort([pos](std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> a, std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> b)
	{
			D3DXVECTOR3 a_dis = pos - ((CommonProcess*)std::get<0>(a))->GetPosition();
			D3DXVECTOR3 b_dis = pos - ((CommonProcess*)std::get<0>(b))->GetPosition();

			return D3DXVec3LengthSq(&a_dis) < D3DXVec3LengthSq(&b_dis);
	});

	
}

void CameraRenderer::CalcCulling(std::list<std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>>> gameobject, int* result)
{
	std::vector<FrustumCullStructuredBuffer> str_buf;
	str_buf.reserve(gameobject.size());

	for (auto obj : gameobject)
	{
		D3DXVECTOR3 position3 = ((CommonProcess*)std::get<0>(obj))->GetPosition();
		D3DXVECTOR3 scale3 = ((CommonProcess*)std::get<0>(obj))->GetScale();

		str_buf.push_back({D3DXVECTOR4(position3.x, position3.y, position3.z, 1.0f), D3DXVECTOR4(scale3.x, scale3.y, scale3.z, 1.0f) });
	}

	m_FrustumStructuredBuffer->UpdateBuffer(str_buf.data(), str_buf.size());
	m_FrustumStructuredBuffer->CSSetStructuredBuffer(0);

	D3DXVECTOR3 planes[6];
	CalculateFrustumPlanes(planes);

	D3DXVECTOR4 info[6];
	for (int i = 0; i < 6; i++)
	{
		info[i] = D3DXVECTOR4(planes[i].x, planes[i].y, planes[i].z, 1.0f);
	}
	m_FrustumCullCameraInfoCBuffer->UpdateBuffer(info);
	m_FrustumCullCameraInfoCBuffer->CSSetCBuffer(10);

	CDxRenderer::GetRenderer()->SetComputeShader(m_CSShader->GetShaderCS().Get());
	m_FrustumCullUAVBuffer->CSSetUnorderedAccessView(0);
	//コンピュートシェーダー実行
	CDxRenderer::GetRenderer()->GetDeviceContext()->Dispatch(32, 1, 1);
	
	m_FrustumCullUAVBuffer->CopyBuffer(result, gameobject.size());

}

void CameraRenderer::ClearGameObjectList()
{

	m_TransparentList.clear();
	m_SpriteList.clear();
	m_OpacityList.clear();
}

void CameraRenderer::DrawGBuffer()
{
	int size = m_OpacityList.size();
	int* result = new int[size];

	CalcCulling(m_OpacityList, result);

	int index = 0;

	for (auto obj : m_OpacityList) {
		
		std::get<0>(obj)->Draw(std::get<1>(obj));
		
		
	}

	delete[] result;
}

void CameraRenderer::DrawTransparent()
{
	int size = m_TransparentList.size();
	int* result = new int[size];

	CalcCulling(m_TransparentList, result);

	int index = 0;

	for (auto obj : m_TransparentList) {
	
		std::get<0>(obj)->Draw(std::get<1>(obj));
		
		
	}


	delete[] result;
}

void CameraRenderer::Draw2D()
{
	for (auto obj : m_SpriteList) {
		std::get<0>(obj)->Draw(std::get<1>(obj));
	}
	/*for (auto obj : m_GameObjects2D) {
		obj->Draw();
	}*/
}

void CameraRenderer::DrawPostEffectToOpacity()
{
	for (auto effect : m_PostEffectToOpacity) {
		effect->Draw();
	}
}

void CameraRenderer::DrawPostEffectToAll()
{
	for (auto effect : m_PostEffectToAll) {
		effect->Draw();
	}
}


void CameraRenderer::SetSkyBox(SkyBox* obj)
{
	m_SkyBox = obj;

	m_SkyBox->DrawByCubeMap();

	ID3D11ShaderResourceView* irradiance = m_SkyBox->GetIrradianceMap();
	ID3D11ShaderResourceView* prefilter = m_SkyBox->GetSpecularMap();
	ID3D11ShaderResourceView* brdflut = m_SkyBox->GetBrdfLUTMap();

	m_DeferredRenderPass->UseEnvMap(irradiance, prefilter, brdflut);

}




void CameraRenderer::SetVPCBuffer(D3DXVECTOR3 pos, D3DXVECTOR3 lookat, D3DXVECTOR3 up)
{

	CAMERA_CBUFFER camera_cbuffer;
	m_CameraInfoValue.CameraPos = D3DXVECTOR4(pos.x, pos.y, pos.z, 1.0f);
	camera_cbuffer.CameraPos = m_CameraInfoValue.CameraPos;

	D3DXMATRIX mtxView, mtxProjection, mtxTransView, mtxTransProj, mtxInverseView, mtxInverseVP, mtxInverseProj;
	D3DXMatrixLookAtLH(&mtxView, &pos, &lookat, &up);
	m_CameraInfoValue.View = mtxView;

	D3DXMatrixTranspose(&mtxTransView, &mtxView);
	camera_cbuffer.TransView = mtxTransView;

	D3DXMatrixInverse(&mtxInverseView, NULL, &mtxView);
	camera_cbuffer.InverseView = mtxInverseView;

	D3DXMatrixPerspectiveFovLH(&mtxProjection, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);
	m_CameraInfoValue.Projection = mtxProjection;

	D3DXMatrixTranspose(&mtxTransProj, &mtxProjection);
	camera_cbuffer.TransProjection = mtxTransProj;

	D3DXMatrixInverse(&mtxInverseProj, NULL, &mtxProjection);
	camera_cbuffer.InverseProjection = mtxInverseProj;

	D3DXMatrixMultiply(&mtxInverseVP, &mtxView, &mtxProjection);
	D3DXMatrixInverse(&mtxInverseVP, NULL, &mtxInverseVP);
	camera_cbuffer.InverseVP = mtxInverseVP;

	D3DXMATRIX worldViewProjection;
	D3DXMatrixOrthoOffCenterLH(&worldViewProjection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	m_CameraInfoValue.WVP = worldViewProjection;

	D3DXMatrixTranspose(&worldViewProjection, &worldViewProjection);
	camera_cbuffer.TransWVP = worldViewProjection;

	m_CameraCBuffer->UpdateBuffer(&camera_cbuffer);
	m_CameraCBuffer->VSSetCBuffer(1);
	m_CameraCBuffer->PSSetCBuffer(1);
	m_CameraCBuffer->GSSetCBuffer(1);
	m_CameraCBuffer->CSSetCBuffer(1);

}

//戻り値: [0] = Left, [1] = Right, [2] = Down, [3] = Up, [4] = Near, [5] = Far
//法線を返す
void CameraRenderer::CalculateFrustumPlanes(D3DXVECTOR3 ret[6])
{

	for (int i = 0; i < 4; i++)
	{
		//平面の方程式参照
		//ax * by + cz + d = 0
		float a, b, c, d;
		
		int index = i / 2;

		if (i % 2 == 0)
		{
			a = m_CameraInfoValue.Projection._41 - m_CameraInfoValue.Projection.m[index][0];
			b = m_CameraInfoValue.Projection._42 - m_CameraInfoValue.Projection.m[index][1];
			c = m_CameraInfoValue.Projection._43 - m_CameraInfoValue.Projection.m[index][2];
			d = m_CameraInfoValue.Projection._44 - m_CameraInfoValue.Projection.m[index][3];
		}									 
		else								 
		{									 
			a = m_CameraInfoValue.Projection._41 + m_CameraInfoValue.Projection.m[index][0];
			b = m_CameraInfoValue.Projection._42 + m_CameraInfoValue.Projection.m[index][1];
			c = m_CameraInfoValue.Projection._43 + m_CameraInfoValue.Projection.m[index][2];
			d = m_CameraInfoValue.Projection._44 + m_CameraInfoValue.Projection.m[index][3];
		}

		ret[i] = -D3DXVECTOR3(a, b, c);
		D3DXVec3Normalize(&ret[i], &ret[i]);
	}

	//near
	{
		float a, b, c, d;

		a = m_CameraInfoValue.Projection._41 + m_CameraInfoValue.Projection._31;
		b = m_CameraInfoValue.Projection._42 + m_CameraInfoValue.Projection._32;
		c = m_CameraInfoValue.Projection._43 + m_CameraInfoValue.Projection._33;
		d = m_CameraInfoValue.Projection._44 + m_CameraInfoValue.Projection._34;

		ret[4] = -D3DXVECTOR3(a, b, c);
		D3DXVec3Normalize(&ret[4], &ret[4]);
		
		ret[4] = QuaXVec3(((Camera*)this)->GetRotation(), ret[4]);
	}

	//far
	{
		float a, b, c, d;

		a = m_CameraInfoValue.Projection._41 - m_CameraInfoValue.Projection._31;
		b = m_CameraInfoValue.Projection._42 - m_CameraInfoValue.Projection._32;
		c = m_CameraInfoValue.Projection._43 - m_CameraInfoValue.Projection._33;
		d = m_CameraInfoValue.Projection._44 - m_CameraInfoValue.Projection._34;

		ret[5] = -D3DXVECTOR3(a, b, c);
		D3DXVec3Normalize(&ret[5], &ret[5]);

		ret[5] = QuaXVec3(((Camera*)this)->GetRotation(), ret[5]);
	}
}

