#pragma once
#include "../Resources/ObjectTag.h"
#include <list>
#include "../Resources/DevelopEnum.h"
#include "DeferredRenderer.h"
#include "../Resources/SkyBox.h"
#include <string>
#include "../Manager/ManagerLight.h"
#include <type_traits>
#include <memory>
#include "../Resources/CMaterial.h"

enum RenderPattern {
	eForward = 0,
	eDeffard,
};

class CBuffer;
class CLight;
class DeferredRenderer;
class CPostEffect;
class SkyBox;
class GBufferPass;
class CRenderer;
class StructuredBuffer;
class UnorderedAccessView;
class CShader;

class CameraRenderer {

private:
	static std::unique_ptr<DeferredRenderer> m_DeferredRenderPass;
	static std::unique_ptr<CPostEffect>  m_ToneMapPass;
	static std::unique_ptr<ManagerLight> m_LightPass;
	static std::unique_ptr<GBufferPass>  m_GBufferPass;
	static std::unique_ptr<CBuffer>		 m_CameraCBuffer;

	static std::unique_ptr<StructuredBuffer>	m_FrustumStructuredBuffer;
	static std::unique_ptr<CBuffer>				m_FrustumCullInfoCBuffer;
	static std::unique_ptr<UnorderedAccessView> m_FrustumCullUAVBuffer;
	static std::shared_ptr<CShader>				m_CSShader;

	static const unsigned int MAX_CULLING_OBJECT = 1000;

	SkyBox*				  m_SkyBox;
	RenderPattern			   m_RenderPattern;
	
	std::list<std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>>> m_TransparentList;
	std::list<std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>>> m_SpriteList;
	std::list<std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>>> m_OpacityList;

	std::list<CPostEffect *> m_PostEffectToOpacity;
	std::list<CPostEffect *> m_PostEffectToAll;


	int m_TagMask = TAG_EVERYTHING;
	CAMERA_INFO m_CameraInfoValue;

	void CalcRenderingOrder(std::list<CGameObject *> gameobject[]);
	void CalcCulling(std::list<std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>>> gameobject, int* result);

	void ClearGameObjectList();

	void DrawGBuffer();
	void DrawTransparent();
	void Draw2D();
	void DrawPostEffectToOpacity();
	void DrawPostEffectToAll();


public:
	static void SetUpCameraRenderer();
	static void Release();

	CameraRenderer();
	virtual ~CameraRenderer() {};

	void SetUpRenderer();
	void Uninit();
	void DrawRenderer(std::list<CGameObject *> gameobject[]);

	void SetSkyBox(SkyBox* obj);
	
	//forward/DeferredêÿÇËë÷Ç¶Ç…égÇ§(Ç‹Çæñ¢ëŒâû)
	void SetRenderType(RenderPattern pattern) {
		m_RenderPattern = pattern;
	}

	void SetPostEffect(CPostEffect* obj, PostEffectType type) {
		switch (type)
		{
		case eOnlyOpacity:
			m_PostEffectToOpacity.push_back(obj);
			break;
		case eAll:
			m_PostEffectToAll.push_back(obj);
			break;
		default:
			break;
		}
	}

	void SetVPCBuffer(D3DXVECTOR3 pos, D3DXVECTOR3 lookat, D3DXVECTOR3 up);
	void SetVPCIdentity();

	template <typename T>
	T* GetPostEffect(PostEffectType type)
	{
		switch (type)
		{
		case eOnlyOpacity:
			for (auto post : m_PostEffectToOpacity) {
				if (typeid(*post) == typeid(T)) {
					return (T*)post;
				}
			}
			break;
		case eAll:
			for (auto post : m_PostEffectToAll) {
				if (typeid(*post) == typeid(T)) {
					return (T*)post;
				}
			}
			break;
		default:
			break;
		}

		return nullptr;
	}

	CAMERA_INFO GetCameraInfo() { return m_CameraInfoValue; }
};