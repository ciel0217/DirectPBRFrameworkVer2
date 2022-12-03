#include "CScene.h"
#include "Camera.h"
#include "../Renderer/CameraRenderer.h"
#include "LinerOctree.h"

void CScene::Config()
{
	for (auto camera : m_SceneCameras) {
		camera->Config();
	}

	for (int i = 0; i < eMaxLayer; i++) {
		for (auto gameobject : m_GameObjects[i]) {
			gameobject->Config();
		}
	}
}

void CScene::Init()
{
	for (auto camera : m_SceneCameras) {
		camera->Init();
	}

	for (int i = 0; i < eMaxLayer; i++) {
		for (auto gameobject : m_GameObjects[i]) {
			gameobject->Init();
		}
	}

	if(m_LinerOctree.get())
		m_LinerOctree->Init();
}

void CScene::Uninit()
{
	for (auto camera : m_SceneCameras) {
		camera->Uninit();
	}

	for (int i = 0; i < eMaxLayer; i++) {
		for (auto gameobject : m_GameObjects[i]) {
			gameobject->Uninit();
		}

		m_GameObjects[i].remove_if([](CGameObject* obj) {
			return obj->Release();
			});
		m_GameObjects[i].clear();
	}

}

void CScene::Update()
{
	for (auto camera : m_SceneCameras)
	{
		camera->Update();
	}
	
	for (int i = 0; i < eMaxLayer; i++) 
	{
		//オブジェクトを削除
		m_GameObjects[i].remove_if([](CGameObject* obj) 
		{
			bool destroy = obj->Destroy();
			if (destroy) {
				delete obj;
			}
			return destroy;
		});

		for (auto gameobject : m_GameObjects[i])
		{
			gameobject->Update();
			if (eMaxLayer == e3DObject && m_LinerOctree.get())
				m_LinerOctree->CalcOctree(gameobject);
		}
	}
	HitCheck();
	

}

void CScene::HitCheck()
{
	if (m_LinerOctree.get())
	{
		m_ManagerCollisionDetection->CollisionCheck(m_LinerOctree->GetObjectList());
		m_LinerOctree->ClearList();
	}else
		m_ManagerCollisionDetection->CollisionCheck(m_GameObjects[e3DObject]);

	
}

void CScene::Draw()
{
	for (auto camera : m_SceneCameras) {
		camera->DrawRenderer(m_GameObjects);
	}
}
