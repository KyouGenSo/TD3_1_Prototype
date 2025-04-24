#include "ResultScene.h"

#include <Input.h>
#include <SceneManager.h>
#include <SpriteBasic.h>

void ResultScene::Initialize()
{
    press_ = std::make_unique<Sprite>();
    press_->Initialize("press_space_text.png");
    press_->SetPos(Vector2(300, 600));
}

void ResultScene::Finalize()
{
}

void ResultScene::Update()
{
    press_->Update();
    if (Input::GetInstance()->TriggerKey(DIK_SPACE))
    {
        SceneManager::GetInstance()->ChangeScene("title");
    }
}

void ResultScene::Draw()
{
    SpriteBasic::GetInstance()->SetCommonRenderSetting();
    press_->Draw();
}

void ResultScene::DrawImGui()
{
}
