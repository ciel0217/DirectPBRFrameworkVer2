#include "TestScene.h"
#include "Framework/Resources/DevelopEnum.h"
#include "Framework/Resources/DirectionalLight.h"
#include "Framework/Resources/SkyBox.h"
#include "Framework/Resources/CCubeMap.h"
#include "Ocean.h"
#include "GameSceneCamera.h"
#include "Test3D.h"
#include "Test2D.h"
#include "TestAnimation.h"
//posteffectñ¢äÆê¨

void TestScene::SetObject()
{
	m_CurrentMainCamera = AddGameObject<GameSceneCamera>(eBackMost);
	AddGameObject<Test3D>(e3DObject)->SetInitPosition(D3DXVECTOR3(0.0f, 0.0f, 10000.0f));
	/*Test2D* test2d = AddGameObject<Test2D>(e2DObject);
	test2d->SetInitPosition(D3DXVECTOR3(250.0f, 250.0f, 0.0f));
	test2d->SetInitScale(D3DXVECTOR3(500.0f, 500.0f, 0.0f));*/

	

	AddGameObject<TestAnimation>(e3DObject)->SetInitPosition(D3DXVECTOR3(0.0f, 0.0f, 50.0f));

	AddGameObject<SkyBox>(e3DObject)->SetInitScale(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f));
	AddGameObject<DirectionalLight>(e3DObject);
	
}
