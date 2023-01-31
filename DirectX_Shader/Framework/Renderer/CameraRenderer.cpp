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

#include <vector>
#include <memory>

#define VIEW_NEAR_Z (0.3f)
#define VIEW_FAR_Z  (10000000.0f)

std::unique_ptr<DeferredRenderer>         CameraRenderer::m_DeferredRenderPass = nullptr;
std::unique_ptr<CPostEffect>             CameraRenderer::m_ToneMapPass = nullptr;
std::unique_ptr<ManagerLight>		      CameraRenderer::m_LightPass = nullptr;
std::unique_ptr<GBufferPass>			  CameraRenderer::m_GBufferPass = nullptr;

std::unique_ptr<CBuffer>				  CameraRenderer::m_CameraCBuffer = nullptr;



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
	//staticÇ∂Ç·Ç»Ç¢Ç‚Ç¬Çâï˙
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
	CalcCulling();

	CDxRenderer::GetRenderer()->SetDepthEnable(true);

	CDxRenderer::GetRenderer()->SetViewPortDefault();
	CDxRenderer::GetRenderer()->ClearRenderTextureSceneByDeffard();
	CDxRenderer::GetRenderer()->SetRenderTargetByDeffard();

	m_GBufferPass->Draw(m_OpacityList);
	
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
				/*std::vector<std::shared_ptr<CMaterial>> materials = object->GetMaterials();
				for (auto material : materials) {
					if (material->GetRenderQueue() <= DrawObjectRenderQueue::eOpacity) {
						m_GameObjectsOpacity.push_back(object);
					}
					else if (material->GetRenderQueue() == DrawObjectRenderQueue::e2D) {
						m_GameObjects2D.push_back(object);
					}
					else {
						m_GameObjectsTransparent.push_back(object);
					}
				}*/
				
			}
		}
	}
	D3DXVECTOR3 pos = m_CameraPos;

	//SortÇÃóDêÊèáà ( 1 : renderqueue   2 : ÉJÉÅÉâÇ∆ÇÃãóó£(âìÇ¢ÇŸÇ§Ç™êÊÇ…ï`âÊ))
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

	//ãﬂÇ¢ÇŸÇ§Ç©ÇÁï`âÊ
	m_OpacityList.sort([pos](std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> a, std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> b)
	{
			D3DXVECTOR3 a_dis = pos - ((CommonProcess*)std::get<0>(a))->GetPosition();
			D3DXVECTOR3 b_dis = pos - ((CommonProcess*)std::get<0>(b))->GetPosition();

			return D3DXVec3LengthSq(&a_dis) < D3DXVec3LengthSq(&b_dis);
	});

	/*m_TransparentList.sort([pos](std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> a, std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> b)
	{
		if (std::get<2>(a)->GetRenderQueue() != std::get<2>(b)->GetRenderQueue())return false;

		D3DXVECTOR3 a_dis = pos - ((CommonProcess*)std::get<0>(a))->GetPosition();
		D3DXVECTOR3 b_dis = pos - ((CommonProcess*)std::get<0>(b))->GetPosition();

		return D3DXVec3LengthSq(&a_dis) > D3DXVec3LengthSq(&b_dis);
	});*/


	
	////ãﬂÇ¢ÇŸÇ§Ç©ÇÁï`âÊ(less)
	//m_GameObjectsOpacity.sort([pos](CommonProcess* a, CommonProcess* b)
	//	{
	//		D3DXVECTOR3 a_dis = pos - a->GetPosition();
	//		D3DXVECTOR3 b_dis = pos - b->GetPosition();

	//		return D3DXVec3LengthSq(&a_dis) < D3DXVec3LengthSq(&b_dis);
	//	});

	////âìÇ¢ÇŸÇ§Ç©ÇÁï`âÊ(greater)
	//m_GameObjectsTransparent.sort([pos](CommonProcess* a, CommonProcess* b)
	//	{
	//		D3DXVECTOR3 a_dis = pos - a->GetPosition();
	//		D3DXVECTOR3 b_dis = pos - b->GetPosition();

	//		return D3DXVec3LengthSq(&a_dis) > D3DXVec3LengthSq(&b_dis);
	//	});
}

void CameraRenderer::CalcCulling()
{
	
}

void CameraRenderer::ClearGameObjectList()
{
	/*m_GameObjects2D.clear();
	m_GameObjectsOpacity.clear();
	m_GameObjectsTransparent.clear();*/

	m_TransparentList.clear();
	m_SpriteList.clear();
	m_OpacityList.clear();
}

void CameraRenderer::DrawTransparent()
{
	for (auto obj : m_TransparentList) {
		std::get<0>(obj)->Draw(std::get<1>(obj));
	}
	/*for (auto obj : m_GameObjectsTransparent) {
		obj->Draw();
	}*/
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
	m_CameraPos = pos;
	CAMERA_CBUFFER camera_cbuffer;
	camera_cbuffer.CameraPos = D3DXVECTOR4(m_CameraPos.x, m_CameraPos.y, m_CameraPos.z, 1.0f);

	D3DXMATRIX mtxView, mtxProjection, mtxTransView, mtxTransProj, mtxInverseView, mtxInverseProj;
	D3DXMatrixLookAtLH(&mtxView, &pos, &lookat, &up);
	D3DXMatrixTranspose(&mtxTransView, &mtxView);
	camera_cbuffer.View = mtxTransView;

	D3DXMatrixInverse(&mtxInverseView, NULL, &mtxView);
	camera_cbuffer.InverseView = mtxInverseView;

	D3DXMatrixPerspectiveFovLH(&mtxProjection, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);
	D3DXMatrixTranspose(&mtxTransProj, &mtxProjection);
	camera_cbuffer.Projection = mtxTransProj;

	D3DXMatrixInverse(&mtxInverseProj, NULL, &mtxProjection);
	camera_cbuffer.InverseProjection = mtxInverseProj;

	D3DXMATRIX worldViewProjection;
	D3DXMatrixOrthoOffCenterLH(&worldViewProjection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&worldViewProjection, &worldViewProjection);
	camera_cbuffer.WVP = worldViewProjection;

	m_CameraCBuffer->UpdateBuffer(&camera_cbuffer);
	m_CameraCBuffer->VSSetCBuffer(1);
	m_CameraCBuffer->PSSetCBuffer(1);
	m_CameraCBuffer->GSSetCBuffer(1);

}

void CameraRenderer::SetVPCIdentity()
{
}

