#include "GameScene.h"

#include "Draw2D.h"
#include "ModelManager.h"
#include "Object3dBasic.h"
#include <Type/Singleton.h>
#include "SpriteBasic.h"

void GameScene::Initialize() {
    ModelManager::GetInstance()->LoadModel("AnimatedCube.gltf");
    pCollisionManager_ = Singleton<CollisionManager>::GetInstance();

    player_ = std::make_unique<Player>();
    player_->Initialize();

	ModelManager::GetInstance()->LoadModel("cube.gltf");
	enemyManager_.Initialize();

	ModelManager::GetInstance()->LoadModel("bigCube.gltf");
	boss_ = std::make_unique<Boss>();
	boss_->Initialize();

    camera_ = std::make_unique<FollowCamera>();
    camera_->Initialize();
    camera_->SetTarget(&player_->GetTransform());

    guiLvUP_ = std::make_unique<GUI_LvUP>();
    guiLvUP_->OnNotify("lvup");

    minimap_ = make_unique<Minimap>();
    minimap_->Initialize();
    minimap_->Register(player_.get());
    minimap_->SetSize({-30, 0, -30}, {30, 0, 30});

    terrain_ = std::make_unique<Terrain>();
    terrain_->Initialize();
}

void GameScene::Finalize() {
    terrain_->Finalize();
    player_->Finalize();
	boss_->Finalize();
	enemyManager_.Finalize();
    camera_->Finalize();
}

void GameScene::Update() {
    terrain_->Update();

    player_->Update();
    camera_->Update();

    guiLvUP_->Update();
    minimap_->Update();


	boss_->Update();
	enemyManager_.Update();
    pCollisionManager_->Update();
}

void GameScene::Draw() {
    Draw2D::GetInstance()->DrawGrid(100.0f, 20.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    Draw2D::GetInstance()->Draw();
    Draw2D::GetInstance()->Reset();

	//------------------背景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();

    //-------------------Modelの描画-------------------//
    // 3Dモデル共通描画設定
    Object3dBasic::GetInstance()->SetCommonRenderSetting();
    terrain_->Draw();
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
}
