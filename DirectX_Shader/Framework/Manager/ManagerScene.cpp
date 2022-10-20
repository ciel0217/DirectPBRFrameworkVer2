#include "../Manager/ManagerScene.h"
#include "CScene.h"

ManagerScene* ManagerScene::_instance = nullptr;

void ManagerScene::Config()
{
	m_CurrentScene->Config();
}

void ManagerScene::Init()
{
	m_CurrentScene->Init();
}

void ManagerScene::Uninit()
{
	m_CurrentScene->Uninit();
}

void ManagerScene::Update()
{
	m_CurrentScene->Update();
}

void ManagerScene::Draw()
{
	m_CurrentScene->Draw();

	CheckNextScene();
}


void ManagerScene::CheckNextScene()
{
	if (m_NextScene) {
		m_CurrentScene->Uninit();
		delete m_CurrentScene;

		m_CurrentScene = m_NextScene;
		m_CurrentScene->Config();
		m_CurrentScene->Init();
		m_NextScene = nullptr;
	}
}