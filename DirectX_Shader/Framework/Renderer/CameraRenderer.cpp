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
std::unique_ptr<StructuredBuffer>			CameraRenderer::m_FrustumCullCameraPlanesStructuredBuffer = nullptr;
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

	if (!m_FrustumCullCameraPlanesStructuredBuffer)
		m_FrustumCullCameraPlanesStructuredBuffer.reset(StructuredBuffer::CreateStructuredBuffer(sizeof(PLANE), 6));

	if (!m_FrustumCullUAVBuffer)
		m_FrustumCullUAVBuffer.reset(UnorderedAccessView::CreateUnorderedAccessView
		(
			sizeof(int),
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
				object->SetSelf(obj);
				std::vector<CMaterial*> materials = object->GetMaterials();
				unsigned int material_count = 0;
				for (auto mat : materials) {
	
					if (mat->GetRenderQueue() <= DrawObjectRenderQueue::eOpacity) {
						m_OpacityList.push_back(std::make_tuple(obj, material_count, mat));
					}
					else if (mat->GetRenderQueue() == DrawObjectRenderQueue::e2D) {
						m_SpriteList.push_back(std::make_tuple(obj, material_count, mat));
					}
					else {
						m_TransparentList.push_back(std::make_tuple(obj, material_count, mat));
					}
					material_count++;
				}
			}
		}
	}
	D3DXVECTOR3 pos = m_CameraInfoValue.CameraPos;

	//Sortの優先順位( 1 : renderqueue   2 : カメラとの距離(遠いほうが先に描画))
	m_TransparentList.sort([pos](std::tuple<CGameObject*, unsigned int, CMaterial*> a, std::tuple<CGameObject*, unsigned int, CMaterial*> b)
	{
			int a_render = std::get<2>(a)->GetRenderQueue();
			int b_render = std::get<2>(b)->GetRenderQueue();

			if (a_render != b_render)
				return a_render < b_render;
			else
			{
				D3DXVECTOR3 a_dis = pos - std::get<0>(a)->GetPosition();
				D3DXVECTOR3 b_dis = pos - std::get<0>(b)->GetPosition();

				return D3DXVec3LengthSq(&a_dis) > D3DXVec3LengthSq(&b_dis);
			}
			
	});
	
	//近いほうから描画
	m_OpacityList.sort([pos](std::tuple<CGameObject*, unsigned int, CMaterial*> a, std::tuple<CGameObject*, unsigned int, CMaterial*> b)
	{
			D3DXVECTOR3 a_dis = pos - std::get<0>(a)->GetPosition();
			D3DXVECTOR3 b_dis = pos - std::get<0>(b)->GetPosition();

			return D3DXVec3LengthSq(&a_dis) < D3DXVec3LengthSq(&b_dis);
	});

	
}

void CameraRenderer::CalcCulling(std::list<std::tuple<CGameObject*, unsigned int, CMaterial*>> gameobject, int* result)
{
	CDxRenderer::GetRenderer()->SetComputeShader(m_CSShader->GetShaderCS().Get());

	std::vector<FrustumCullStructuredBuffer> str_buf;
	str_buf.reserve(gameobject.size());

	for (auto obj : gameobject)
	{
		D3DXVECTOR3 position3 = std::get<0>(obj)->GetPosition();
		CRenderer* render = dynamic_cast<CRenderer*>(std::get<0>(obj));
		
		D3DXVECTOR3 scale3 = render->GetBounds();

		str_buf.push_back({D3DXVECTOR4(position3.x, position3.y, position3.z, 1.0f), D3DXVECTOR4(scale3.x, scale3.y, scale3.z, 1.0f) });
	}

	m_FrustumStructuredBuffer->UpdateBuffer(str_buf.data(), str_buf.size());
	m_FrustumStructuredBuffer->CSSetStructuredBuffer(3);

	PLANE	planes[6];
	CalculateFrustumPlanes(planes);

	m_FrustumCullCameraPlanesStructuredBuffer->UpdateBuffer(planes, 6);
	m_FrustumCullCameraPlanesStructuredBuffer->CSSetStructuredBuffer(4);

	
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

	for (auto obj : m_OpacityList) 
	{
		if (result[index] == 6)
		{
			CRenderer* render = dynamic_cast<CRenderer*>(std::get<0>(obj));
			render->Draw(std::get<1>(obj));
		}
		index++;
	}

	delete[] result;
}

void CameraRenderer::DrawTransparent()
{
	int size = m_TransparentList.size();
	int* result = new int[size];

	CalcCulling(m_TransparentList, result);

	int index = 0;

	if (m_SkyBox)
		m_SkyBox->Draw(0);

	for (auto obj : m_TransparentList)
	{
		if (result[index] == 6)
		{
			CRenderer* render = dynamic_cast<CRenderer*>(std::get<0>(obj));
			render->Draw(std::get<1>(obj));
		}
		

		index++;
	}

	delete[] result;
}

void CameraRenderer::Draw2D()
{
	for (auto obj : m_SpriteList)
	{
		{
			CRenderer* render = dynamic_cast<CRenderer*>(std::get<0>(obj));
			render->Draw(std::get<1>(obj));
		}
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

D3DXVECTOR3 CameraRenderer::CalcPlane(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c)
{
	D3DXVECTOR3 ab = b - a;
	D3DXVECTOR3 ac = c - a;

	D3DXVECTOR3 ret;
	D3DXVec3Cross(&ret, &ab, &ac);

	return ret;
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

	D3DXMatrixPerspectiveFovLH(&mtxProjection, 1.04f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);
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
void CameraRenderer::CalculateFrustumPlanes(PLANE ret[6])
{

	D3DXMATRIX vp, view, proj;
	view = m_CameraInfoValue.View;
	proj = m_CameraInfoValue.Projection;
	//D3DXMatrixTranspose(&view, &view);
	//D3DXMatrixTranspose(&proj, &proj);
	D3DXMatrixMultiply(&vp, &view, &proj);
	D3DXMatrixInverse(&vp, NULL, &vp);
	
	D3DXVECTOR3 frustum_corners[8], normal[6];
	frustum_corners[0] = D3DXVECTOR3(-1, -1, 0);
	frustum_corners[1] = D3DXVECTOR3(1, -1, 0);
	frustum_corners[2] = D3DXVECTOR3(-1, 1, 0);
	frustum_corners[3] = D3DXVECTOR3(1, 1, 0);
	frustum_corners[4] = D3DXVECTOR3(-1, -1, 1);
	frustum_corners[5] = D3DXVECTOR3(1, -1, 1);
	frustum_corners[6] = D3DXVECTOR3(-1, 1, 1);
	frustum_corners[7] = D3DXVECTOR3(1, 1, 1);

	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&frustum_corners[i], &frustum_corners[i], &vp);

	normal[0] = CalcPlane(frustum_corners[4], frustum_corners[0], frustum_corners[2]);
	normal[1] = CalcPlane(frustum_corners[1], frustum_corners[5], frustum_corners[7]);
	normal[2] = CalcPlane(frustum_corners[4], frustum_corners[1], frustum_corners[0]);
	normal[3] = CalcPlane(frustum_corners[2], frustum_corners[3], frustum_corners[6]);
	normal[4] = CalcPlane(frustum_corners[0], frustum_corners[1], frustum_corners[2]);
	normal[5] = CalcPlane(frustum_corners[6], frustum_corners[7], frustum_corners[5]);

	for (int i = 0; i < 6; i++)
	{
		D3DXVec3Normalize(&normal[i], &normal[i]);
		ret[i].Normal = D3DXVECTOR4(normal[i].x, normal[i].y, normal[i].z, 1.0f);
		if (i < 4)
			ret[i].Distance = 0.0f;
		else if (i == 4)
			ret[i].Distance = VIEW_NEAR_Z;
		else
			ret[i].Distance = VIEW_FAR_Z;
			
	}

	//for (int i = 0; i < 4; i++)
	//{
	//	//平面の方程式参照
	//	//ax * by + cz + d = 0
	//	float a, b, c, d;
	//	
	//	int index = i / 2;
	//	
	//	
	//	if (i % 2 == 0)
	//	{
	//	
	//		a = proj._41 - proj.m[index][0];
	//		b = proj._42 - proj.m[index][1];
	//		c = proj._43 - proj.m[index][2];
	//		d = proj._44 - proj.m[index][3];
	//	}									 
	//	else								 
	//	{				
	//	
	//		a = proj._41 + proj.m[index][0];
	//		b = proj._42 + proj.m[index][1];
	//		c = proj._43 + proj.m[index][2];
	//		d = proj._44 + proj.m[index][3];
	//	}
	//	

	///*	D3DXVec4Normalize(&ret[i].Normal, &ret[i].Normal);

	//	D3DXVECTOR4 v = view.m[3];
	//	ret[i].Distance = -D3DXVec4Dot(&ret[i].Normal, &v);*/

	//	D3DXVECTOR3 normal  = D3DXVECTOR3(a, b, c);
	//	D3DXVec3Normalize(&normal, &normal);
	//	ret[i].Normal = D3DXVECTOR4(normal.x, normal.y, normal.z, 1.0f);
	//	ret[i].Distance = d;
	//}

	////near
	//{
	//	float a, b, c, d;

	//	a = proj._41 + proj._31;
	//	b = proj._42 + proj._32;
	//	c = proj._43 + proj._33;
	//	d = proj._44 + proj._34;

	//	D3DXVECTOR3 normal = D3DXVECTOR3(a, b, c);
	//	D3DXVec3Normalize(&normal, &normal);
	//	
	//	 normal = QuaXVec3(((Camera*)this)->GetRotation(), normal);
	//	 ret[4].Normal = D3DXVECTOR4(normal.x, normal.y, normal.z, 1.0f);
	//	 ret[4].Distance = VIEW_NEAR_Z;
	//}
	//int hh = 0;
	////far
	//{
	//	float a, b, c, d;

	//	a = proj._41 - proj._31;
	//	b = proj._42 - proj._32;
	//	c = proj._43 - proj._33;
	//	d = proj._44 - proj._34;

	//	D3DXVECTOR3 normal = D3DXVECTOR3(a, b, c);
	//	D3DXVec3Normalize(&normal, &normal);

	//	normal = QuaXVec3(((Camera*)this)->GetRotation(), normal);
	//	ret[5].Normal = D3DXVECTOR4(normal.x, normal.y, normal.z, 1.0f);
	//	ret[5].Distance = VIEW_FAR_Z;
	//}
}

