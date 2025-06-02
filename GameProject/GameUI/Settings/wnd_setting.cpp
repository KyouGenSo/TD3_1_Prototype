#include "wnd_setting.h"

#include <NiGui.h>
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>
#include <GameSystem/SoundManager/SoundManager.h>
#include <Utility/Adaptor.h>
#include <WinApp.h>
#include <imgui.h>

void Wnd_Setting::Initialize()
{
    wndsize_ = { WinApp::clientWidth, WinApp::clientHeight };

    controller_volume_.Initialize("Volume", { 0, -50.0f });
    controller_sens_.Initialize("Sens", { 0, 50.0f });
    controller_volume_.SetMax(1.0f);
    controller_volume_.SetValue(1.0f);
    controller_sens_.SetMax(1.0f);
    controller_sens_.SetValue(1.0f);

    pos_sprite_volume_ = {};
    pos_sprite_volume_.x = -150.0f;
    pos_sprite_volume_.y = -50.0f;

    pos_sprite_sens_ = {};
    pos_sprite_sens_.x = -150.0f;
    pos_sprite_sens_.y = 50.0f;

    sprite_text_volume_ = std::make_unique<Sprite>();
    sprite_text_volume_->Initialize("volumeText.png");
    sprite_text_volume_->SetSize(NiUtil::Adaptor(NiVec2(97.0f, 63.0f) * 0.5f));
    sprite_text_sens_ = std::make_unique<Sprite>();
    sprite_text_sens_->Initialize("mouseSensitiveText.png");
    sprite_text_sens_->SetSize(NiUtil::Adaptor(NiVec2(240.0f, 64.0f) * 0.5f));


    id_callback_change_display_ = notifier_->RegisterCallback("RequestOpenSetting", [this](std::any _flag)
    {
        bool open = std::any_cast<bool>(_flag);

        // デルタタイム
        if (isDisplay_) notifier_->Notify("OnWindowOpen", !isDisplay_);
        if (open) DeltaTimeManager::GetInstance()->SetDeltaTime(1, 0.0f);
        else DeltaTimeManager::GetInstance()->SetDeltaTime(1, 1.0f / 60.0f);
        isDisplay_ = open;
    });

    auto center = NiGui_StandardPoint::Center;

    size_background_= { 600.0f, 300.0f };

    arg_window_.id = "setting_background";
    arg_window_.textureName = "white.png";
    arg_window_.size = size_background_;
    arg_window_.position = {};
    arg_window_.color = {};
    arg_window_.anchor = center;
    arg_window_.pivot = center;

    sprite_background_ = std::make_unique<Sprite>();
    sprite_background_->Initialize(arg_window_.textureName);
    sprite_background_->SetColor(pColorResolver_->Resolve(ColorName::Background).toVector4());
    sprite_background_->SetSize(NiUtil::Adaptor(size_background_));
}

void Wnd_Setting::Finalize()
{
    notifier_->UnregisterCallback("RequestOpenSetting", id_callback_change_display_);
}

void Wnd_Setting::Update()
{
    wndsize_ = { WinApp::clientWidth, WinApp::clientHeight };

    pos_background_ = wndsize_ / 2.0f - size_background_ / 2.0f;
    sprite_background_->SetPos(NiUtil::Adaptor(pos_background_));
    sprite_background_->Update();

    this->Show();

    // スプライトテキストの座標更新
    auto size = sprite_text_volume_->GetSize();
    size.y *= 0.5f;
    sprite_text_volume_->SetPos(NiUtil::Adaptor((wndsize_ * 0.5f) + pos_sprite_volume_ - NiUtil::Adaptor(size)));
    sprite_text_volume_->Update();

    size = sprite_text_sens_->GetSize();
    size.y *= 0.5f;
    sprite_text_sens_->SetPos(NiUtil::Adaptor((wndsize_ * 0.5f) + pos_sprite_sens_ - NiUtil::Adaptor(size)));
    sprite_text_sens_->Update();


    if (controller_volume_.IsChanged()) SoundManager::GetInstance()->SetVolumeMultiply(controller_volume_.GetValue());
    if (controller_sens_.IsChanged()) notifier_->Notify("ChangedSens", controller_sens_.GetValue());
}

void Wnd_Setting::Draw2d()
{
    if (!isDisplay_) return;
    sprite_background_->Draw();
    controller_volume_.Draw2d();
    controller_sens_.Draw2d();
    sprite_text_volume_->Draw();
    sprite_text_sens_->Draw();
}

void Wnd_Setting::OnNotify(const std::string& _name, const std::string& _event)
{
    if (_name != "setting" && _name != "everyone") return;

    auto dtm = DeltaTimeManager::GetInstance();
    if (_event == "open")
    {
        dtm->SetDeltaTime(1, 0.0f);
        notifier_->Notify("OnWindowOpen", true);
        isDisplay_ = true;
    }
    else if (_event == "close")
    {
        dtm->SetDeltaTime(1, 1.0f / 60.0f);
        if (isDisplay_) notifier_->Notify("OnWindowOpen", false);
        isDisplay_ = false;
    }
    else if (_event == "trigger")
    {
        if (isDisplay_) dtm->SetDeltaTime(1, 1.0f / 60.0f);
        else dtm->SetDeltaTime(1, 0.0f);
        notifier_->Notify("OnWindowOpen", isDisplay_);
        isDisplay_ = !isDisplay_;
    }
}

void Wnd_Setting::Show()
{
    if (!isDisplay_) return;

    NiGui::BeginDiv(arg_window_);
    controller_volume_.Update();
    controller_sens_.Update();
    NiGui::EndDiv();
}