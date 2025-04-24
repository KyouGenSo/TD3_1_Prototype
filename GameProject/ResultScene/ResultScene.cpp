#include "ResultScene.h"

#include <Input.h>
#include <SceneManager.h>

void ResultScene::Initialize()
{
}

void ResultScene::Finalize()
{
}

void ResultScene::Update()
{
    if (Input::GetInstance()->TriggerKey(DIK_SPACE))
    {
        SceneManager::GetInstance()->ChangeScene("title");
    }
}

void ResultScene::Draw()
{
}

void ResultScene::DrawImGui()
{
}
