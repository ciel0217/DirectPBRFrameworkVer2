#include "TestScene.h"
#include "enum.h"
#include "DirectionalLight.h"
#include "SkyBox.h"
#include "CCubeMap.h"
#include "Ocean.h"
#include "GameSceneCamera.h"

//posteffectñ¢äÆê¨

void TestScene::SetObject()
{
	m_CurrentMainCamera = AddGameObject<GameSceneCamera>(eBackMost);
	//AddGameObject<test2D>(e2DObject);
	//AddGameObject<Test3D>(e3DObject)->SetPosition(D3DXVECTOR3(0.0f, .0f, 50.0f));
	/*Ocean* ocean = AddGameObject<Ocean>(e3DObject);
	ocean->SetInitPosition(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	ocean->SetInitScale(D3DXVECTOR3(100000.0f, 1.0f, 100000.0f));
	
	Player* test = AddGameObject<Player>(e3DObject);
	test->SetInitPosition(D3DXVECTOR3(0.0f, 500.0f, 10.0f));
	test->SetInitScale(D3DXVECTOR3(5.0f, 5.0f, 5.0f));

	EnemyManager* manager = AddGameObject<EnemyManager>(e3DObject);

	Reticle* reticle = AddGameObject<Reticle>(e3DObject);
	reticle->SetInitScale(D3DXVECTOR3(60.0f, 60.0f, 60.0f));

	TargetMark* mark = AddGameObject<TargetMark>(e3DObject);
	mark->SetInitScale(D3DXVECTOR3(1000.0f, 1000.0f, 1.0f));
	
	Rader* rader = AddGameObject<Rader>(e2DObject);
	rader->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH/8.0f, 400.0f, 0.0f));
	rader->SetInitScale(D3DXVECTOR3(220.0f, 220.0f, 0.0f));

	MissionMessage* message = AddGameObject<MissionMessage>(e2DObject);
	message->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH, 50.0f, 0.0f));
	message->SetInitScale(D3DXVECTOR3(150.0f, 50.0f, 0.0f));

	UIDestroy* destroy = AddGameObject<UIDestroy>(e2DObject);
	destroy->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH/4.0f, SCREEN_HEIGHT / 3.0f, 0.0f));
	destroy->SetInitScale(D3DXVECTOR3(200.0f,50.0f,1.0f));

	UIPitchUp* pitchup = AddGameObject<UIPitchUp>(e2DObject);
	pitchup->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	pitchup->SetInitScale(D3DXVECTOR3(200.0f, 50.0f, 1.0f));

	Score* score = AddGameObject<Score>(e2DObject);
	score->SetInitPosition(D3DXVECTOR3(800.0f, 30.0f, 0.0f));
	score->SetInitScale(D3DXVECTOR3(32.0f, 64.0f, 0.0f));

	HudBullet* hud = AddGameObject<HudBullet>(e2DObject);
	hud->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH/2.0f + 300.0f, SCREEN_HEIGHT/2.0f + 200.0f, 0.0f));
	hud->SetInitScale(D3DXVECTOR3(200.0f, 150.0f,0.0f));

	DamageEffect* damage = AddGameObject<DamageEffect>(e2DObject);
	damage->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f, 0.0f));
	damage->SetInitScale(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	Pause* pause = AddGameObject<Pause>(e2DObject);
	pause->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	pause->SetInitScale(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	pause->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));

	OpenBlackBG* open = AddGameObject<OpenBlackBG>(e2DObject);
	open->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f,0.0f));
	open->SetInitScale(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	CloseBlackBG* close = AddGameObject<CloseBlackBG>(e2DObject);
	close->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH / 2.0f + SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f, 0.0f));
	close->SetInitScale(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));*/

	AddGameObject<SkyBox>(e3DObject)->SetInitScale(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f));
	AddGameObject<DirectionalLight>(e3DObject);
	
	
	//AddPostEffect<PostEffectBloom>();
}
