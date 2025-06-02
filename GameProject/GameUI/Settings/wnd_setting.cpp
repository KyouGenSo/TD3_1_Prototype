#include "wnd_setting.h"

#include <NiGui.h>
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>
#include <GameSystem/SoundManager/SoundManager.h>
#include <Utility/Adaptor.h>
#include <WinApp.h>

void Wnd_Setting::Initialize()
{
    controller_volume_.Initialize("Volume", { 0, -50.0f });
    controller_sens_.Initialize("Sens", { 0, 50.0f });

    controller_volume_.SetMax(1.0f);
    controller_volume_.SetValue(0.8f);
    controller_sens_.SetMax(1.0f);
    id_callback_change_display_ = notifier_->RegisterCallback("RequestOpenSetting", [this](std::any _flag)
    {
        bool open = std::any_cast<bool>(_flag);
        isDisplay_ = open;
    });

    auto center = NiGui_StandardPoint::Center;

    NiVec2 bgSize = { 600.0f, 300.0f };
    NiVec2 wndsize = { WinApp::clientWidth, WinApp::clientHeight };
    arg_window_.id = "setting_background";
    arg_window_.textureName = "white.png";
    arg_window_.size = bgSize;
    arg_window_.position = {};
    arg_window_.color = {};
    arg_window_.anchor = center;
    arg_window_.pivot = center;

    NiVec2 bgpos = wndsize / 2.0f - bgSize / 2.0f;

    sprite_background_ = std::make_unique<Sprite>();
    sprite_background_->Initialize(arg_window_.textureName);
    sprite_background_->SetPos(NiUtil::Adaptor(bgpos));
    sprite_background_->SetColor(pColorResolver_->Resolve(ColorName::Gray).toVector4());
    sprite_background_->SetSize(NiUtil::Adaptor(bgSize));
}

void Wnd_Setting::Finalize()
{
    notifier_->UnregisterCallback("RequestOpenSetting", id_callback_change_display_);
}

void Wnd_Setting::Update()
{
    this->Show();
    sprite_background_->Update();
    SoundManager::GetInstance()->SetVolumeMultiply(controller_volume_.GetValue());
}

void Wnd_Setting::Draw2d()
{
    if (!isDisplay_) return;
    sprite_background_->Draw();
    controller_volume_.Draw2d();
    controller_sens_.Draw2d();
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