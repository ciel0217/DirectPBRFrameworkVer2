#include "TestScene.h"
#include "Framework/Resources/DevelopEnum.h"
#include "Framework/Resources/DirectionalLight.h"
#include "Framework/Resources/SkyBox.h"
#include "Framework/Resources/CCubeMap.h"
#include "Ocean.h"
#include "GameSceneCamera.h"
#include "Test3D.h"
#include "TestAnimation.h"
//posteffectñ¢äÆê¨

void TestScene::SetObject()
{
	m_CurrentMainCamera = AddGameObject<GameSceneCamera>(eBackMost);
	//AddGameObject<Test3D>(e3DObject)->SetInitPosition(D3DXVECTOR3(0.0f, 2.0f, 10.0f));
	AddGameObject<TestAnimation>(e3DObject)->SetInitPosition(D3DXVECTOR3(0.0f, 0.0f, 50.0f));

	AddGameObject<SkyBox>(e3DObject)->SetInitScale(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f));
	AddGameObject<DirectionalLight>(e3DObject);
	
}
