#pragma once
#include "TestScene.h"
#include "TitleScene.h"
#include "ResultScene.h"
#include <vector>

class CScene;

class ManagerScene
{
private:
	CScene* m_CurrentScene;
	CScene* m_NextScene = nullptr;
	static ManagerScene* _instance;


	void CheckNextScene();
public:
	ManagerScene() { m_CurrentScene = new TitleScene();}
	
	static void Create() {
		if (_instance == nullptr) {
			_instance = new ManagerScene();
			_instance->GetCurrentScene()->Config();
			_instance->GetCurrentScene()->Init();
		}
	}

	static ManagerScene* GetInstance() { return _instance; }
	CScene* GetCurrentScene() { return m_CurrentScene; }

	void Config();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	template <typename T>
	T* ChangeScene() {
		T* scene = new T();
		m_NextScene = (CScene*)scene;

		return scene;
	}

	Camera* GetCurrentSceneCamera() { return m_CurrentScene->GetCurrentMainCamera(); }

};

