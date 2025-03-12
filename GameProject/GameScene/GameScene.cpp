#include "GameScene.h"

#include "Draw2D.h"
#include "ModelManager.h"
#include "Object3dBasic.h"
#include <Type/Singleton.h>
#include "SpriteBasic.h"
#include "ImGuiManager.h"


void GameScene::Initialize() {

    directLightParam_ = {
        .direction = { 0.0f, -1.0f, 0.0f },
        .color = { 1.0f, 1.0f, 1.0f, 1.0f },
        .lightType = 1,
        .intensity = 2.0f
    };

    ModelManager::GetInstance()->LoadModel("AnimatedCube.gltf");
    pCollisionManager_ = Singleton<CollisionManager>::GetInstance();

    // プレイヤーの初期化
    player_ = std::make_unique<Player>();
    player_->Initialize();

    // Camera
    camera_ = std::make_unique<FollowCamera>();
    camera_->Initialize();
    camera_->SetTarget(&player_->GetTransform());

    guiLvUP_ = std::make_unique<GUI_LvUP>();
    guiPauseMenu_ = std::make_unique<GUI_PauseMenu>();

    // Observer登録
    player_->AddObserver(guiLvUP_.get());
    player_->AddObserver(guiPauseMenu_.get());

    // Minimap
    minimap_ = make_unique<Minimap>();
    minimap_->Initialize();
    minimap_->SetSize({-30, 0, -30}, {30, 0, 30});
    minimap_->Register(player_.get());

    // 敵の初期化
	ModelManager::GetInstance()->LoadModel("cube.gltf");
    enemyManager_.SetMinimap(minimap_.get());
    enemyManager_.Initialize();

    // ボスの初期化
	ModelManager::GetInstance()->LoadModel("bigCube.gltf");
	boss_ = std::make_unique<Boss>();
	boss_->Initialize();

    // Terrain
    terrain_ = std::make_unique<Terrain>();
    terrain_->Initialize();

    castle_ = std::make_unique<Castle>();
    castle_->Initialize();
}

void GameScene::Finalize() {
    terrain_->Finalize();
    player_->Finalize();
	boss_->Finalize();
	enemyManager_.Finalize();
    camera_->Finalize();
}

void GameScene::Update() {
    Object3dBasic::GetInstance()->SetDirectionalLight(directLightParam_.direction, directLightParam_.color, directLightParam_.lightType, directLightParam_.intensity);

    terrain_->Update();
    castle_->Update();

    player_->Update();
    camera_->Update();

    guiLvUP_->Update();
    guiPauseMenu_->Update();

	boss_->Update();
	enemyManager_.Update();

    minimap_->Update();

    pCollisionManager_->Update();
}

void GameScene::Draw() {
    //Draw2D::GetInstance()->DrawGrid(100.0f, 20.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    //Draw2D::GetInstance()->Draw();
    //Draw2D::GetInstance()->Reset();

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
	enemyManager_.Draw();

    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();

    minimap_->Draw();
}

void GameScene::DrawImGui() {
    terrain_->ImGui();
    player_->ImGui();
	enemy_->ImGui();
    enemyManager_.ImGui();
    camera_->ImGui();

    ImGui::Begin("Directional Light");
    ImGui::DragFloat3("Direction", &directLightParam_.direction.x, 0.01f);
    ImGui::ColorEdit4("Color", &directLightParam_.color.x);
    ImGui::DragFloat("Intensity", &directLightParam_.intensity, 0.01f);
    ImGui::End();
}
