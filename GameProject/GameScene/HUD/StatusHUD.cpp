#include "StatusHUD.h"



void StatusHUD::Initialize()
{
    input_ = Input::GetInstance();

    hpBar_ = std::make_unique<StatusBar>();
    hpBar_->Initialize("hud/name_hp.png", { 512.0f, 17.0f });
    hpBar_->SetMaxValue(100.0f);
    hpBar_->SetCurrentValue(80.0f);

    bulletBar_ = std::make_unique<StatusBar>();
    bulletBar_->Initialize("hud/name_assault.png", { 159.0f, 10.0f });
    bulletBar_->SetMaxValue(30.0f);
    bulletBar_->SetCurrentValue(30.0f);

    globalVariables_ = GlobalVariables::GetInstance();
    globalVariables_->CreateGroup("StatusHUD");

    globalVariables_->AddItem("StatusHUD", "HPBar_Position", Vector2());
    globalVariables_->AddItem("StatusHUD", "BulletBar_Position", Vector2());
    
    this->Reload();
}

void StatusHUD::Reload()
{
    globalVariables_->LoadFile("StatusHUD");
    Vector2 hpBarPos = globalVariables_->GetValueVec2("StatusHUD", "HPBar_Position");
    Vector2 bulletBarPos = globalVariables_->GetValueVec2("StatusHUD", "BulletBar_Position");
    hpBarPos_ = {hpBarPos.x, hpBarPos.y};
    bulletBarPos_ = { bulletBarPos.x, bulletBarPos.y };

    isReloaded_ = true;
}

void StatusHUD::Update()
{
    #ifdef _DEBUG
    UpdateHotReload();
    #endif // _DEBUG

    if (isReloaded_)
    {
        hpBar_->SetPosition(hpBarPos_);
        bulletBar_->SetPosition(bulletBarPos_);

        isReloaded_ = false;
    }

    hpBar_->Update();
    bulletBar_->Update();
}

void StatusHUD::Draw2D()
{
    hpBar_->Draw2D();
    bulletBar_->Draw2D();
}

void StatusHUD::ImGui()
{
    hpBar_->ImGui();
    bulletBar_->ImGui();
}

void StatusHUD::UpdateHotReload()
{
    if (input_->TriggerKey(DIK_F5))
    {
        this->Reload();
    }
}
