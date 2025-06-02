#include "GameScene.h"

#include "Object3dBasic.h"
#include <Type/Singleton.h>
#include "SpriteBasic.h"
#include "ImGuiManager.h"
#include <GameSystem/StageManager/StageManager.h>
#include <SceneManager.h>
#include <WinApp.h>
#include <NiGui.h>

#include "GPUParticle.h"
#include <functional>

#include "PostEffect.h"


void GameScene::Initialize()
{
    eventTimer_ = EventTimer::GetInstance();
  
    NiGui::ResetItemToArea();
    eventTimer_->BeginEvent("Initialize");

    //ModelManager::GetInstance()->GetModel("rocketBullet.gltf");

    PostEffect::GetInstance()->SetEffectType("RGBSplit");
    PostEffect::GetInstance()->SetRGBSplitIntensity(0.2f);

    // ステージデータの取得
    const auto& currentStageData = StageManager::GetInstance()->GetCurrentStageData();

    
    directLightParam_ = {
        .direction = { .x= 0.0f, .y= -1.0f, .z= 0.0f },
        //.color = {1.000f, 0.254f, 0.254f, 1.000f},
        .color = {.x= 1.000f, .y= 1.0f, .z= 1.0f, .w= 1.000f},
        .lightType = 1,
        .intensity = 2.0f
    };

    eventTimer_->BeginEvent("EmitterManager");

    emitterManager_ = std::make_unique<EmitterManager>(GPUParticle::GetInstance());
    eventTimer_->EndEvent("EmitterManager");


    pCollisionManager_ = Singleton<Collision::Manager>::GetInstance();

    eventTimer_->BeginEvent("Terrain");
    // Terrain
    terrain_ = std::make_unique<Terrain>();
    terrain_->Initialize();

    eventTimer_->EndEvent("Terrain");

    eventTimer_->BeginEvent("Player");
    // プレイヤーの初期化
    player_ = std::make_unique<Player>();
    player_->SetEmitter(emitterManager_.get());
    player_->Initialize();
    player_->SetFloor(terrain_->GetFloorHeight());
    player_->SetTransform(currentStageData.playerTransform);
    player_->SetEmitter(emitterManager_.get());

    eventTimer_->EndEvent("Player");

    // Camera
    camera_ = std::make_unique<FollowCamera>();
    camera_->Initialize();
    camera_->SetTarget(&player_->GetTransform());

    freeLookCamera_ = std::make_unique<FreeLookCamera>();
    freeLookCamera_->Initialize();

    eventTimer_->BeginEvent("Chain");
    // ChainViewModel
    chainViewModel_ = std::make_unique<ChainViewModel>();
    chainViewModel_->Initialize();

    eventTimer_->EndEvent("Chain");

    // GUIの初期化
    guiLvUP_ = std::make_unique<GUI_LvUP>();
    guiLvUP_->Initialize();
    guiPauseMenu_ = std::make_unique<GUI_PauseMenu>();
    guiChain_ = std::make_unique<GUI_Chain>();
    guiChain_->Initialize();
    guiChain_->SetViewModel(chainViewModel_.get());

    wnd_setting_ = std::make_unique<Wnd_Setting>();
    wnd_setting_->Initialize();

    // Observer登録
    player_->AddObserver(guiLvUP_.get());
    player_->AddObserver(guiPauseMenu_.get());
    player_->AddObserver(guiChain_.get());

    // Minimap
    minimap_ = std::make_unique<Minimap>();
    minimap_->Initialize();
    minimap_->SetSize({ -100, 0, -400.f }, { 100, 0, 400 });
    minimap_->Register(player_.get());

    // Castle
    castle_ = std::make_unique<Castle>();
    castle_->Initialize();
    castle_->SetTransform(currentStageData.castleTransform);
    minimap_->Register(castle_.get());

    eventTimer_->BeginEvent("Enemy");
    // 敵の初期化
    enemyManager_ = std::make_unique<EnemyManager>();
    //enemyManager_->SetMinimap(minimap_.get());
    enemyManager_->Initialize(player_.get(), castle_.get());
    enemyManager_->SetEmitter(emitterManager_.get());
    enemyManager_->SetMinimap(minimap_.get());

    // ボスの初期化
    boss_ = std::make_unique<Boss>();
    boss_->Initialize();
    boss_->SetTransform(currentStageData.bossTransform);

    eventTimer_->EndEvent("Enemy");

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

    emitterManager_->CreateSphereEmitter("thunder", {0,0, 0},  3, 250, 0);
    emitterManager_->SetEmitterActive("thunder", false);
    //emitterManager_->SetEmitterVelocityRange("explosion", {-0.1f, 0.1f}, {-0.1f,  0.1f}, {-0.1f, 0.1f});
    emitterManager_->SetEmitterColor("thunder", {1.f, 0.f, 0.f, 1});
    emitterManager_->SetEmitterStartColor("thunder", {1, 0.6f, 0.2f, 1});
    emitterManager_->SetEmitterEndColor("thunder", {1.f, 1.f, 0.f, 1});
    emitterManager_->SetEmitterScaleRange("thunder", {0.4f, 0.4f}, {0.4f, 0.4f});

    emitterManager_->CreateSphereEmitter("hit", {0,0,0}, 5, 100, 0);
    emitterManager_->SetEmitterActive("hit", false);
    emitterManager_->SetEmitterColor("hit", {1.f, 0.f, 0.f, 1});
    emitterManager_->SetEmitterScaleRange("hit", {0.1f, 0.1f}, {0.1f, 0.1f});
  
    statusHUD_->GetHpBar()->SetMaxValue(player_->getStatusCurrent().getMaxHp());

    // リサイズ時コールバック登録
    handle_onresizes_ = {
        WinApp::GetInstance()->RegisterOnResizeFunc(std::bind(&StatusHUD::OnResized, statusHUD_.get(), std::placeholders::_1)),
        WinApp::GetInstance()->RegisterOnResizeFunc(std::bind(&CountDown::OnResize, countDown_.get(), std::placeholders::_1)),
        WinApp::GetInstance()->RegisterOnResizeFunc(std::bind(&GUI_LvUP::OnResize, guiLvUP_.get(), std::placeholders::_1)),
        WinApp::GetInstance()->RegisterOnResizeFunc([this](Vector2 size) { reticle_->SetPos({size.x / 2.0f, size.y / 2.0f}); }),
    };

    // ReinforcementManagerの初期化
    auto* reinforcementManager_ = ReinforcementManager::GetInstance();
    reinforcementManager_->Initialize(reinforcementManager_->kFilename_json_status_);

    eventTimer_->BeginEvent("Sound");
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
    eventTimer_->EndEvent("Sound");

    reticle_ = std::make_unique<Sprite>();
    reticle_->Initialize("cross.png");
    reticle_->SetAnchorPoint({0.5f, 0.5f});
    reticle_->SetPos({static_cast<float>(WinApp::clientWidth) / 2.f, static_cast<float>(WinApp::clientHeight) / 2.f});
    reticle_->SetSize({32.f,32.f});

    f11Sprite_ = std::make_unique<Sprite>();
    f11Sprite_->Initialize("F11.png");
    f11SpritePos_ = { .x = 0.f, .y = 0.f };
    f11Sprite_->SetPos(f11SpritePos_);
    f11Sprite_->SetSize({ 250.f, 40.f });

    pauseKeySp_ = std::make_unique<Sprite>();
    pauseKeySp_->Initialize("pauseKey.png");
    pauseKeySp_->SetPos(pauseKeySpPos_);
    pauseKeySp_->SetSize({ pauseKeySp_->GetSize().x * 0.5f, pauseKeySp_->GetSize().y * 0.5f });

    statusKeySpPos_ = { .x = 44.89f, .y = 259.21f };
    statusKeySp_ = std::make_unique<Sprite>();
    statusKeySp_->Initialize("statusKey.png");
    statusKeySp_->SetPos(statusKeySpPos_);
    statusKeySp_->SetSize({ statusKeySp_->GetSize().x * 0.5f, statusKeySp_->GetSize().y * 0.5f });

    weaponChainKeySpPos_ = { .x = 42.8f, .y = 304.6f };
    weaponChainKeySp_ = std::make_unique<Sprite>();
    weaponChainKeySp_->Initialize("weaponChainKey.png");
    weaponChainKeySp_->SetPos(weaponChainKeySpPos_);
    weaponChainKeySp_->SetSize({ weaponChainKeySp_->GetSize().x * 0.5f, weaponChainKeySp_->GetSize().y * 0.5f });

    eventTimer_->EndEvent("Initialize");
}

void GameScene::Finalize()
{
    for (auto& handle : handle_onresizes_)
    {
        WinApp::GetInstance()->UnregisterOnResizeFunc(handle);
    }

    emitterManager_->RemoveAllEmitters();

    terrain_->Finalize();
    player_->Finalize();
    boss_->Finalize();
    enemyManager_->Finalize();
    camera_->Finalize();
    player_->Finalize();
    wnd_setting_->Finalize();

    soundGroup_->Finalize();
}

void GameScene::Update()
{
    if (boss_->GetIsValid())
    {
        directLightParam_.color = { .x= 1.0f, .y= 0.254f, .z= 0.254f, .w= 1.0f };
    }
    else
    {
        directLightParam_.color = { .x= 1.0f, .y= 1.0f, .z= 1.0f, .w= 1.0f };
    }

    Object3dBasic::GetInstance()->SetDirectionalLight(directLightParam_.direction, directLightParam_.color, directLightParam_.lightType, directLightParam_.intensity);
    ReinforcementManager::GetInstance()->Update();
    this->UpdateInputCommands();

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

    if (enemyManager_->GetLastTurn() <= enemyManager_->GetCurrentTurn()){
        boss_->SetIsValid(true);
        minimap_->Register(boss_.get());
    }
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
    wnd_setting_->Update();
    eventTimer_->Measure("detect", [&]{pCollisionManager_->Detect(); });
    eventTimer_->Measure("event", [&]{pCollisionManager_->ProcessEvent(); });
    emitterManager_->Update();
    // プレイヤーのHPのUIを更新
    statusHUD_->GetHpBar()->SetMaxValue(player_->getStatusCurrent().getMaxHp());
    *(statusHUD_->GetHpBar()) = player_->getStatusCurrent().getHp();
    // プレイヤーのXPのUIを更新
    statusHUD_->GetXPBar()->SetMaxValue(player_->getXPMax());
    *(statusHUD_->GetXPBar()) = player_->getXP();
    // 城のHPのUIを更新
    statusHUD_->GetCastleHpBar()->SetMaxValue(castle_->getStatusCurrent().getMaxHp());
    *(statusHUD_->GetCastleHpBar()) = castle_->getStatusCurrent().getHp();
    reticle_->Update();
    f11SpritePos_.x = static_cast<float>(WinApp::clientWidth) - f11Sprite_->GetSize().x - 10.f;
    f11Sprite_->SetPos(f11SpritePos_);
    f11Sprite_->Update();
    pauseKeySp_->SetPos(pauseKeySpPos_);
    pauseKeySp_->Update();
    statusKeySp_->SetPos(statusKeySpPos_);
    statusKeySp_->Update();
    weaponChainKeySp_->SetPos(weaponChainKeySpPos_);
    weaponChainKeySp_->Update();

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
    player_->DrawDebug();

    GPUParticle::GetInstance()->Draw();

    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();


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

    countDown_->Draw2D();
    minimap_->Draw();
    statusHUD_->Draw2D();
    enemyManager_->Draw2d();
    boss_->Draw2d();
    reticle_->Draw();
    f11Sprite_->Draw();
    pauseKeySp_->Draw();
    statusKeySp_->Draw();
    weaponChainKeySp_->Draw();
    wnd_setting_->Draw2d();
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
    castle_->ImGui();
    chainViewModel_->ImGui();

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
    if (castle_->getStatusCurrent().getHp() <= 0)
    {
        SceneManager::GetInstance()->ChangeScene("result");
    }
}

void GameScene::UpdateInputCommands()
{
    auto input = Input::GetInstance();

    std::vector<IObserver*> observers;
    observers.push_back(guiLvUP_.get());
    observers.push_back(guiPauseMenu_.get());
    observers.push_back(guiChain_.get());
    observers.push_back(wnd_setting_.get());

    if (input->TriggerKey(DIK_ESCAPE))
    {
        for (auto observer : observers)
        {
            observer->OnNotify("everyone", "close");
        }
        guiPauseMenu_->OnNotify("pause_menu", "toggle");
    }
    if (input->TriggerKey(DIK_TAB))
    {
        guiLvUP_->OnNotify("lvup", "toggle");
    }
    if (input->TriggerKey(DIK_C))
    {
        guiChain_->OnNotify("chain", "toggle");
    }
}