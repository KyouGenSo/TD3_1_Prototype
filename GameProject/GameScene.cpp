#include "GameScene.h"

#include "Draw2D.h"
#include "ModelManager.h"
#include "Object3dBasic.h"
#include "Singleton.h"
#include "SpriteBasic.h"

void GameScene::Initialize() {
    ModelManager::GetInstance()->LoadModel("AnimatedCube.gltf");
    pCollisionManager_ = Singleton<CollisionManager>::GetInstance();

    player_ = std::make_unique<Player>();
    player_->Initialize();

	ModelManager::GetInstance()->LoadModel("cube.gltf");
	enemy_ = std::make_unique<Enemy>();
    enemy_->Initialize();

	ModelManager::GetInstance()->LoadModel("bigCube.gltf");
	boss_ = std::make_unique<Boss>();
	boss_->Initialize();

    camera_ = std::make_unique<FollowCamera>();
    camera_->Initialize();
    camera_->SetTarget(&player_->GetTransform());

    guiLvUP_ = std::make_unique<GUI_LvUP>();
    guiLvUP_->OnNotify("lvup");
}

void GameScene::Finalize() {
    player_->Finalize();
	enemy_->Finalize();
	boss_->Finalize();
    camera_->Finalize();
}

void GameScene::Update() {
    camera_->Update();
    player_->Update();
    //guiLvUP_->Update();

	enemy_->Update();
	boss_->Update();
    pCollisionManager_->Update();
}

void GameScene::Draw() {
    //------------------背景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();

    //-------------------Modelの描画-------------------//
    // 3Dモデル共通描画設定
    Object3dBasic::GetInstance()->SetCommonRenderSetting();
    player_->Draw();
	enemy_->Draw();
	boss_->Draw();

    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();

    Draw2D::GetInstance()->DrawGrid(100.0f, 20.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

void GameScene::DrawImGui() {
    player_->ImGui();
    camera_->ImGui();
}
