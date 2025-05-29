#include "StatusHUD.h"


void StatusHUD::Initialize()
{
    input_ = Input::GetInstance();

    hpBar_ = std::make_unique<StatusBar>();
    hpBar_->Initialize("hud/name_hp.png", { 512.0f, 17.0f }, false);
    hpBar_->SetMaxValue(100.0f);
    hpBar_->SetCurrentValue(80.0f);

    castleHpBar_ = std::make_unique<StatusBar>();
    castleHpBar_->Initialize("hud/name_castle.png", { 159.0f, 10.0f }, false);
    castleHpBar_->SetMaxValue(30.0f);
    castleHpBar_->SetCurrentValue(30.0f);

    xpBar_ = std::make_unique<StatusBar>();
    xpBar_->Initialize("", { 512.0f, 8.0f });

    globalVariables_ = GlobalVariables::GetInstance();
    globalVariables_->CreateGroup("StatusHUD");

    globalVariables_->AddItem("StatusHUD", "Standard_DisplaySize", Vector2());
    globalVariables_->AddItem("StatusHUD", "HPBar_Position", Vector2());
    globalVariables_->AddItem("StatusHUD", "BulletBar_Position", Vector2());
    globalVariables_->AddItem("StatusHUD", "XPBar_Position", Vector2());
    
    this->Reload();

    retio_.x = WinApp::clientWidth / standardDisplaySize_.x;
    retio_.y = WinApp::clientHeight / standardDisplaySize_.y;
}

void StatusHUD::Reload()
{
    globalVariables_->LoadFile("StatusHUD");
    Vector2 std_disp_size = globalVariables_->GetValueVec2("StatusHUD", "Standard_DisplaySize");
    Vector2 hpBarPos = globalVariables_->GetValueVec2("StatusHUD", "HPBar_Position");
    Vector2 bulletBarPos = globalVariables_->GetValueVec2("StatusHUD", "BulletBar_Position");
    Vector2 xpBarPos = globalVariables_->GetValueVec2("StatusHUD", "XPBar_Position");

    standardDisplaySize_ = { std_disp_size.x, std_disp_size.y };
    hpBarPos_ = {hpBarPos.x, hpBarPos.y};
    bulletBarPos_ = { bulletBarPos.x, bulletBarPos.y };
    xpBarPos_ = { xpBarPos.x, xpBarPos.y };

    isReloaded_ = true;
}

void StatusHUD::Update()
{
    #ifdef _DEBUG
    UpdateHotReload();
    #endif // _DEBUG

    if (isReloaded_)
    {
        hpBar_->SetPosition(hpBarPos_ * retio_);
        castleHpBar_->SetPosition(bulletBarPos_ * retio_);
        xpBar_->SetPosition(xpBarPos_ * retio_);

        isReloaded_ = false;
    }

    hpBar_->Update();
    castleHpBar_->Update();
    xpBar_->Update();
}

void StatusHUD::Draw2D()
{
    hpBar_->Draw2D();
    castleHpBar_->Draw2D();
    xpBar_->Draw2D();
}

void StatusHUD::ImGui()
{
    hpBar_->ImGui();
    castleHpBar_->ImGui();
    xpBar_->ImGui();
}

void StatusHUD::OnResized(Vector2 _size)
{
    NiVec2 size = { _size.x, _size.y };
    retio_ = { size.x / standardDisplaySize_.x, size.y / standardDisplaySize_.y };
    hpBar_->SetPosition(hpBarPos_ * retio_);
    castleHpBar_->SetPosition(bulletBarPos_ * retio_);
    xpBar_->SetPosition(xpBarPos_ * retio_);
}

void StatusHUD::UpdateHotReload()
{
    if (input_->TriggerKey(DIK_F5))
    {
        this->Reload();
    }
}
