#include "StatusHUD.h"



void StatusHUD::Initialize()
{
    input_ = Input::GetInstance();

    hpBar_ = std::make_unique<StatusBar>();
    hpBar_->Initialize("hud/name_hp.png");

    hpBar_->SetMaxValue(100.0f);
    hpBar_->SetCurrentValue(80.0f);

    globalVariables_ = GlobalVariables::GetInstance();
    globalVariables_->CreateGroup("StatusHUD");

    globalVariables_->AddItem("StatusHUD", "HPBar_Position", Vector2());
    
    this->Reload();
}

void StatusHUD::Reload()
{
    globalVariables_->LoadFile("StatusHUD");
    Vector2 hpBarPos = globalVariables_->GetValueVec2("StatusHUD", "HPBar_Position");
    hpBarPos_ = {hpBarPos.x, hpBarPos.y};

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

        isReloaded_ = false;
    }

    hpBar_->Update();
}

void StatusHUD::Draw2D()
{
    hpBar_->Draw2D();
}

void StatusHUD::ImGui()
{
    hpBar_->ImGui();
}

void StatusHUD::UpdateHotReload()
{
    if (input_->TriggerKey(DIK_F5))
    {
        this->Reload();
    }
}
