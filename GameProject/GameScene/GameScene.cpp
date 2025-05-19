#include "GameScene.h"

#include "ModelManager.h"
#include "Object3dBasic.h"
#include <Type/Singleton.h>
#include "SpriteBasic.h"
#include "ImGuiManager.h"
#include <GameSystem/StageManager/StageManager.h>
#include <SceneManager.h>
#include <WinApp.h>


#include "GPUParticle.h"
#include <Audio.h>
#include <Vector2.h>
#include <Draw2D.h>


void GameScene::Initialize()
{
	// ステージデータの取得
	const auto& currentStageData = StageManager::GetInstance()->GetCurrentStageData();
	eventTimer_ = EventTimer::GetInstance();


	directLightParam_ = {
		.direction = { 0.0f, -1.0f, 0.0f },
		.color = { 1.0f, 1.0f, 1.0f, 1.0f },
		.lightType = 1,
		.intensity = 2.0f
	};

    emitterManager_ = std::make_unique<EmitterManager>(GPUParticle::GetInstance());

	pCollisionManager_ = Singleton<Collision::Manager>::GetInstance();

	// Terrain
	terrain_ = std::make_unique<Terrain>();
	terrain_->Initialize();

	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetFloor(terrain_->GetFloorHeight());
	player_->SetTransform(currentStageData.playerTransform);
	player_->SetEmitter(emitterManager_.get());

	// Camera
	camera_ = std::make_unique<FollowCamera>();
	camera_->Initialize();
	camera_->SetTarget(&player_->GetTransform());

    freeLookCamera_ = std::make_unique<FreeLookCamera>();
    freeLookCamera_->Initialize();
    //Object3dBasic::GetInstance()->SetCamera(freeLookCamera_->GetCamera());
    //Draw2D::GetInstance()->SetCamera(freeLookCamera_->GetCamera());

    // ChainViewModel
    chainViewModel_ = std::make_unique<ChainViewModel>();
    chainViewModel_->Initialize();

	// GUIの初期化
	guiLvUP_ = std::make_unique<GUI_LvUP>();
	guiPauseMenu_ = std::make_unique<GUI_PauseMenu>();
	guiChain_ = std::make_unique<GUI_Chain>();
	guiChain_->Initialize();
	guiChain_->SetViewModel(chainViewModel_.get());

	// Observer登録
	player_->AddObserver(guiLvUP_.get());
	player_->AddObserver(guiPauseMenu_.get());
	player_->AddObserver(guiChain_.get());

    // Minimap
    minimap_ = std::make_unique<Minimap>();
    minimap_->Initialize();
    minimap_->SetSize({ -30, 0, -30 }, { 30, 0, 30 });
    minimap_->Register(player_.get());

    // Castle
    castle_ = std::make_unique<Castle>();
    castle_->Initialize();
    castle_->SetTransform(currentStageData.castleTransform);

    // 敵の初期化
    enemyManager_ = std::make_unique<EnemyManager>();
    //enemyManager_->SetMinimap(minimap_.get());
    enemyManager_->Initialize(player_.get(), castle_.get());
    enemyManager_->SetEmitter(emitterManager_.get());

    // ボスの初期化
    boss_ = std::make_unique<Boss>();
    boss_->Initialize();
    boss_->SetTransform(currentStageData.bossTransform);

    // TimeKeeper
    timeKeeper_ = std::make_unique<TimeKeeper>();
    timeKeeper_->Initialize();
    timeKeeper_->AddEvent("CountDown", 3.0f);
    timeKeeper_->AddEvent("JunbiPhase", 12.0f);
    timeKeeper_->Load();
    timeKeeper_->Run("JunbiPhase");

    player_->SetChain(chainViewModel_->GetChain());

    gameController_ = std::make_unique<GameController>();
    gameController_->SetPlayerModel(player_.get());
    gameController_->SetGUIChainView(guiChain_.get());

    guiChain_->SetGameController(gameController_.get());
    guiLvUP_->SetGameController(gameController_.get());

    threadpool_ = Threadpool::GetInstance();

    // CountDownの初期化
    countDown_ = std::make_unique<CountDown>();
    countDown_->Initialize();

    // StatusHUDの初期化
    statusHUD_ = std::make_unique<StatusHUD>();
    statusHUD_->Initialize();

    emitterManager_->CreateSphereEmitter("explosion", {0,0, 0},  5, 250, 0);
    emitterManager_->SetEmitterActive("explosion", false);
    //emitterManager_->SetEmitterVelocityRange("explosion", {-0.1f, 0.1f}, {-0.1f,  0.1f}, {-0.1f, 0.1f});
    emitterManager_->SetEmitterColor("explosion", {1.f, 0.f, 0.f, 1});
    emitterManager_->SetEmitterStartColor("explosion", {1, 0, 0, 1});
    emitterManager_->SetEmitterEndColor("explosion", {1.f, 1.f, 0.f, 1});
    emitterManager_->SetEmitterScaleRange("explosion", {0.4f, 0.4f}, {0.4f, 0.4f});

    emitterManager_->CreateSphereEmitter("hit", {0,0,0}, 5, 100, 0);
    emitterManager_->SetEmitterActive("hit", false);
    emitterManager_->SetEmitterColor("hit", {1.f, 0.f, 0.f, 1});
    emitterManager_->SetEmitterScaleRange("hit", {0.1f, 0.1f}, {0.1f, 0.1f});
  
    statusHUD_->GetHpBar()->SetMaxValue(player_->getStatusCurrent().getMaxHp());

    // リサイズ時コールバック登録
    resizeFuncIdx = WinApp::GetInstance()->RegisterOnResizeFunc(std::bind(&StatusHUD::OnResized, statusHUD_.get(), std::placeholders::_1));

    // ReinforcementManagerの初期化
    auto* reinforcementManager_ = ReinforcementManager::GetInstance();
    reinforcementManager_->Initialize(reinforcementManager_->kFilename_json_status_);

    // BGMグループの初期化
    soundGroup_ = std::make_unique<SoundGroup>();
    soundGroup_->Initialize();
    soundGroup_->SetSoundGroupName("BGM");
    soundGroup_->SetInterval(2.0f);
    soundGroup_->AddSound(SoundManager::GetInstance()->SearchSoundData("BGM_Game_0").handle);
    soundGroup_->AddSound(SoundManager::GetInstance()->SearchSoundData("BGM_Game_1").handle);
    soundGroup_->AddSound(SoundManager::GetInstance()->SearchSoundData("BGM_Game_2").handle);
    soundGroup_->AddSound(SoundManager::GetInstance()->SearchSoundData("BGM_Game_3").handle);
    soundGroup_->AddSound(SoundManager::GetInstance()->SearchSoundData("BGM_Game_4").handle);
    soundGroup_->Start();
}

void GameScene::Finalize()
{
    terrain_->Finalize();
    player_->Finalize();
    boss_->Finalize();
    enemyManager_->Finalize();
    camera_->Finalize();
    player_->Finalize();

    soundGroup_->Finalize();
    WinApp::GetInstance()->UnregisterOnResizeFunc(resizeFuncIdx);
}

void GameScene::Update()
{
	Object3dBasic::GetInstance()->SetDirectionalLight(directLightParam_.direction, directLightParam_.color, directLightParam_.lightType, directLightParam_.intensity);

	soundGroup_->Update();

	timeKeeper_->Update();
	chainViewModel_->Update();

    eventTimer_->Measure("Update Terrain", [&]() { terrain_->Update(); });
    eventTimer_->Measure("Update Castle", [&]() { castle_->Update(); });
    eventTimer_->Measure("Update Player", [&]() { player_->Update(); });
    eventTimer_->Measure("Update Camera", [&]() { camera_->Update(); });
    freeLookCamera_->Update();

	eventTimer_->Measure("Update GUI", [&]()
	{
		guiLvUP_->Update();
		guiPauseMenu_->Update();
		guiChain_->Update();
	});

	boss_->Update();
	eventTimer_->Measure("Update EnemyManager", [&]() { enemyManager_->Update(); });
	eventTimer_->Measure("Update Minimap", [&]() { minimap_->Update(); });

	/// タイマーの更新
	if (timeKeeper_->GetRemainTime("JunbiPhase") < 3.0f && !countDown_->IsStart())
	{
		countDown_->Start();
		timeKeeper_->Reset("JunbiPhase");
	}

	countDown_->Update();

	statusHUD_->Update();
	eventTimer_->Measure("detect", [&]{pCollisionManager_->Detect(); });
	eventTimer_->Measure("event", [&]{pCollisionManager_->ProcessEvent(); });

	emitterManager_->Update();
	*(statusHUD_->GetHpBar()) = player_->getStatusCurrent().getHp();

	// ステータスの監視 (ゲームシーンからリザルトシーンへの移行)
	this->MonitorStatus();
}

void GameScene::Draw()
{
	/// ================================== ///
	///              描画処理               ///
	/// ================================== ///
	//------------------背景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();

    //-------------------Modelの描画-------------------//
    // 3Dモデル共通描画設定
    Object3dBasic::GetInstance()->SetCommonRenderSetting();
    terrain_->Draw();
    castle_->Draw();
    player_->Draw();
    boss_->Draw();
    enemyManager_->Draw();
    //camera_->Draw3D();

	GPUParticle::GetInstance()->Draw();

    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();
    countDown_->Draw2D();
    minimap_->Draw();
    statusHUD_->Draw2D();

    //camera_->Draw2D();
}

void GameScene::DrawWithoutEffect()
{
	/// ================================== ///
	///              描画処理               ///
	/// ================================== ///
	//------------------背景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();




	//-------------------Modelの描画-------------------//
	// 3Dモデル共通描画設定
	Object3dBasic::GetInstance()->SetCommonRenderSetting();




	//------------------前景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();


}

void GameScene::DrawImGui()
{
	terrain_->ImGui();
	player_->ImGui();
	enemyManager_->ImGui();
	boss_->ImGui();
	camera_->ImGui();
	guiChain_->ImGui();
	timeKeeper_->ImGui();
	statusHUD_->ImGui();
	soundGroup_->ImGui();

	ImGui::Begin("Directional Light");
	ImGui::DragFloat3("Direction", &directLightParam_.direction.x, 0.01f);
	ImGui::ColorEdit4("Color", &directLightParam_.color.x);
	ImGui::DragFloat("Intensity", &directLightParam_.intensity, 0.01f);
	ImGui::End();
}

void GameScene::MonitorStatus()
{
	if (player_->getStatusCurrent().getHp() <= 0)
	{
		SceneManager::GetInstance()->ChangeScene("result");
	}
}
