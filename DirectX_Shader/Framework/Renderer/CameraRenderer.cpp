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

#include <vector>
#include <memory>

#define VIEW_NEAR_Z (0.3f)
#define VIEW_FAR_Z  (10000000.0f)

std::unique_ptr<DeferredRenderer>         CameraRenderer::m_DeferredRenderPass = nullptr;
std::unique_ptr<CPostEffect>             CameraRenderer::m_ToneMapPass = nullptr;
std::unique_ptr<ManagerLight>		      CameraRenderer::m_LightPass = nullptr;
std::unique_ptr<GBufferPass>			  CameraRenderer::m_GBufferPass = nullptr;

std::unique_ptr<CBuffer>				  CameraRenderer::m_ViewCBuffer = nullptr;
std::unique_ptr<CBuffer>				  CameraRenderer::m_ProjectionCBuffer = nullptr;
std::unique_ptr<CBuffer>				  CameraRenderer::m_CameraPosCBuffer = nullptr;
std::unique_ptr<CBuffer>				  CameraRenderer::m_ViewInverseCBuffer = nullptr;
std::unique_ptr<CBuffer>				  CameraRenderer::m_ProjectionInverseCBuffer = nullptr;


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

	if (!m_ViewCBuffer)
		m_ViewCBuffer.reset(new CBuffer(CBuffer::CreateBuffer(sizeof(D3DMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr)));
	if (!m_ProjectionCBuffer)
		m_ProjectionCBuffer.reset(new CBuffer(CBuffer::CreateBuffer(sizeof(D3DMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr)));

	if (!m_ViewInverseCBuffer)
		m_ViewInverseCBuffer.reset(new CBuffer(CBuffer::CreateBuffer(sizeof(D3DMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr)));
	if (!m_ProjectionInverseCBuffer)
		m_ProjectionInverseCBuffer.reset(new CBuffer(CBuffer::CreateBuffer(sizeof(D3DMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr)));

	if (!m_CameraPosCBuffer)
		m_CameraPosCBuffer.reset(new CBuffer(CBuffer::CreateBuffer(sizeof(D3DXCOLOR), D3D11_BIND_CONSTANT_BUFFER, nullptr)));

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
	//static‚¶‚á‚È‚¢‚â‚Â‚ð‰ð•ú
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

	m_GBufferPass->Draw(m_OpacityList);
	
	CDxRenderer::GetRenderer()->ClearIntermediateBuffer(false);
	CDxRenderer::GetRenderer()->SetRenderTargetIntermediateBuffer(false);

	m_LightPass->SetLightCBuffer();
	m_DeferredRenderPass->Draw();
	
	DrawPostEffectToOpacity();

	CDxRenderer::GetRenderer()->SetRenderTargetIntermediateBuffer(true);
	DrawTransparent();

	CDxRenderer::GetRenderer()->ClearBackBuffor(false);
	CDxRenderer::GetRenderer()->SetRenderTargetBackBuffor(true);

	SetVPCIdentity();

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
				unsigned int count = 0;
				for (auto id : material_ids) {
					std::shared_ptr<CMaterial> material = ManagerMaterial::GetMaterial(id);
					if (material->GetRenderQueue() <= DrawObjectRenderQueue::eOpacity) {
						m_OpacityList.push_back(std::make_tuple(object, count, material));
					}
					else if (material->GetRenderQueue() == DrawObjectRenderQueue::e2D) {
						m_SpriteList.push_back(std::make_tuple(object, count, material));
					}
					else {
						m_TransparentList.push_back(std::make_tuple(object, count, material));
					}

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

	m_TransparentList.sort([](std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> a, std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> b)
	{
			int a_render = std::get<2>(a)->GetRenderQueue();
			int b_render = std::get<2>(b)->GetRenderQueue();

			return a < b;
	});

	D3DXVECTOR3 pos = m_CameraPos;

	m_TransparentList.sort([pos](std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> a, std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>> b)
	{
		if (std::get<2>(a)->GetRenderQueue() != std::get<2>(b)->GetRenderQueue())return false;

		D3DXVECTOR3 a_dis = pos - ((CommonProcess*)std::get<0>(a))->GetPosition();
		D3DXVECTOR3 b_dis = pos - ((CommonProcess*)std::get<0>(b))->GetPosition();

		return D3DXVec3LengthSq(&a_dis) > D3DXVec3LengthSq(&b_dis);
	});


	
	////‹ß‚¢‚Ù‚¤‚©‚ç•`‰æ(less)
	//m_GameObjectsOpacity.sort([pos](CommonProcess* a, CommonProcess* b)
	//	{
	//		D3DXVECTOR3 a_dis = pos - a->GetPosition();
	//		D3DXVECTOR3 b_dis = pos - b->GetPosition();

	//		return D3DXVec3LengthSq(&a_dis) < D3DXVec3LengthSq(&b_dis);
	//	});

	////‰“‚¢‚Ù‚¤‚©‚ç•`‰æ(greater)
	//m_GameObjectsTransparent.sort([pos](CommonProcess* a, CommonProcess* b)
	//	{
	//		D3DXVECTOR3 a_dis = pos - a->GetPosition();
	//		D3DXVECTOR3 b_dis = pos - b->GetPosition();

	//		return D3DXVec3LengthSq(&a_dis) > D3DXVec3LengthSq(&b_dis);
	//	});
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

	D3DXMATRIX mtxView, mtxProjection, mtxTransView, mtxTransProj, mtxInverseView, mtxInverseProj;
	D3DXMatrixLookAtLH(&mtxView, &pos, &lookat, &up);
	D3DXMatrixTranspose(&mtxTransView, &mtxView);
	m_ViewCBuffer->UpdateBuffer(&mtxTransView);
	m_ViewCBuffer->VSSetCBuffer(1);
	m_ViewCBuffer->PSSetCBuffer(1);

	D3DXMatrixPerspectiveFovLH(&mtxProjection, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);
	D3DXMatrixTranspose(&mtxTransProj, &mtxProjection);
	m_ProjectionCBuffer->UpdateBuffer(&mtxTransProj);
	m_ProjectionCBuffer->VSSetCBuffer(2);
	m_ProjectionCBuffer->PSSetCBuffer(2);

	D3DXMatrixInverse(&mtxInverseView, NULL, &mtxView);
	m_ViewInverseCBuffer->UpdateBuffer(&mtxInverseView);
	m_ViewInverseCBuffer->VSSetCBuffer(6);
	m_ViewInverseCBuffer->PSSetCBuffer(6);


	D3DXMatrixInverse(&mtxInverseProj, NULL, &mtxProjection);
	m_ProjectionInverseCBuffer->UpdateBuffer(&mtxInverseProj);
	m_ProjectionInverseCBuffer->VSSetCBuffer(7);
	m_ProjectionInverseCBuffer->PSSetCBuffer(7);

	m_CameraPosCBuffer->UpdateBuffer(pos);
	m_CameraPosCBuffer->PSSetCBuffer(5);
}

void CameraRenderer::SetVPCIdentity()
{
	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	m_ViewCBuffer->UpdateBuffer(&view);
	m_ViewCBuffer->VSSetCBuffer(1);
	m_ViewCBuffer->PSSetCBuffer(1);

	D3DXMATRIX worldViewProjection;
	D3DXMatrixOrthoOffCenterLH(&worldViewProjection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&worldViewProjection, &worldViewProjection);
	m_ProjectionCBuffer->UpdateBuffer(&worldViewProjection);
	m_ProjectionCBuffer->VSSetCBuffer(2);
	
}

