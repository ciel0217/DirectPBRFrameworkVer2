#include "CScene.h"
#include "Camera.h"
#include "../Renderer/CameraRenderer.h"

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
	for (auto camera : m_SceneCameras) {
		camera->Update();
	}

	for (int i = 0; i < eMaxLayer; i++) {
		for (auto gameobject : m_GameObjects[i]) {
			gameobject->Update();
		}
	}

	HitCheck();
	
	for (int i = 0; i < eMaxLayer; i++) {
		m_GameObjects[i].remove_if([](CGameObject* obj) {

			bool destroy = obj->Destroy();
			if (destroy) {
				delete obj;
			}
			return destroy;
		});
	}

}

void CScene::HitCheck()
{
	m_ManagerCollisionDetection->CollisionCheck(m_GameObjects[e3DObject]);
}

void CScene::Draw()
{
	for (auto camera : m_SceneCameras) {
		camera->DrawRenderer(m_GameObjects);
	}
}
