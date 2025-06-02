#include "wnd_chain.h"

#include <NiGui.h>
#include <imgui.h>

#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>
#include <GameSystem/GameController/GameController.h>
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>

void GUI_Chain::Initialize()
{
    NiGui::ResetItemToArea();
    NiGui::SetItemToArea("Assault", "DragItemArea1");
}

void GUI_Chain::OnNotify(const std::string& _name, const std::string& _event)
{
    if (_name != "chain" && _name != "everyone") return;

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
    else if (_event == "toggle")
    {
        if (isDisplay_) dtm->SetDeltaTime(1, 1.0f / 60.0f);
        else dtm->SetDeltaTime(1, 0.0f);
        isDisplay_ = !isDisplay_;
        notifier_->Notify("OnWindowOpen", isDisplay_);
    }
}

void GUI_Chain::Update()
{
    /// 通知されたら表示する
    if (isDisplay_)
    {
        ShowChain();
    }
    if (isConfirm_)
    {
        // チェインを確定する
        chainViewModel_->UpdateChainData({ area1_, area2_, area3_, "" });

        // プレイヤーにも通知する
        gameController_->HandleConfirmChain();

        GameEventNotifier::GetInstance()->Notify("ChainConfirm", nullptr);

        // デルタタイム
        DeltaTimeManager::GetInstance()->SetDeltaTime(1, 1.0f / 60.0f);

        // ウィンドウが閉じたことを通知
        notifier_->Notify("OnWindowOpen", false);


        isConfirm_ = false;
        isDisplay_ = false;
    }
}

void GUI_Chain::ImGui()
{
    ImGui::Begin("Chain Display");
    ImGui::Checkbox("Display", &isDisplay_);
    ImGui::Text("Area1 : %s", area1_.c_str());
    ImGui::Text("Area2 : %s", area2_.c_str());
    ImGui::Text("Area3 : %s", area3_.c_str());
    ImGui::End();
}

void GUI_Chain::ShowChain()
{
    auto center = NiGui_StandardPoint::Center;
    auto lefttop = NiGui_StandardPoint::LeftTop;
    auto confirm = NiGui_ButtonState::Confirm;
    if (NiGui::BeginDiv("Chain", (CHAINDIR_ / TEX_WND_).string(), NiGui::WHITE, { 0, 0 }, { 1200,700 }, center, center))
    {
        if (NiGui::BeginDiv("DragItemHolder", TEX_WHITE_, {}, { 218,190 }, { 758,128 }, lefttop, lefttop))
        {
            area1_ = NiGui::DragItemArea("DragItemArea1", (CHAINDIR_ / TEX_FRAME_).string(), NiGui::WHITE, { 0, 0 }, { 128, 128 }, lefttop, lefttop);
            area2_ = NiGui::DragItemArea("DragItemArea2", (CHAINDIR_ / TEX_FRAME_).string(), NiGui::WHITE, { 210, 0 }, { 128, 128 }, lefttop, lefttop);
            area3_ = NiGui::DragItemArea("DragItemArea3", (CHAINDIR_ / TEX_FRAME_).string(), NiGui::WHITE, { 420, 0 }, { 128, 128 }, lefttop, lefttop);
            area3_ = NiGui::DragItemArea("DragItemArea4", (CHAINDIR_ / TEX_FRAME_).string(), NiGui::WHITE, { 630, 0 }, { 128, 128 }, lefttop, lefttop);

            NiGui::EndDiv();
        }

        if (NiGui::BeginDiv("DragItemAreaDiv", (CHAINDIR_ / TEX_HOLDER_).string(), NiGui::WHITE, {222,361}, {755,128}, lefttop, lefttop))
        {
            NiGui::EndDiv();
        }

        NiGui::DragItem("RocketLauncher", (ICONDIR_ / TEX_ROCKETLAUNCHER_).string(), NiGui::WHITE, { -240, 70 }, { 100, 100 }, center, center);
        NiGui::DragItem("Assault", (ICONDIR_ / TEX_ASSAULT_).string(), NiGui::WHITE, { 0, 70 }, { 100, 100 }, center, center);
        NiGui::DragItem("MachineGun", (ICONDIR_ / TEX_SUBMACHINEGUN_).string(), NiGui::WHITE, { 240, 70 }, { 100, 100 }, center, center);


        if (NiGui::Button("ConfirmChain", (CHAINDIR_ / TEX_BUTTON_CONFIRM_).string(), NiGui::WHITE, { 642, 533 }, { 292,84 }, {}, lefttop, lefttop) == confirm)
        {
            if (CheckValidChain())
            {
                isConfirm_ = true;
            }

        }
        NiGui::EndDiv();
    }
}

bool GUI_Chain::CheckValidChain()
{
    std::array<std::string, 3> chain = { area1_, area2_, area3_ };
    bool isChainValid = true;
    bool isChainEmpty = false;

    if (chain.front().empty())
    {
        isChainValid = false;
    }

    for (const auto& area : chain)
    {
        if (!area.empty() && isChainEmpty)
        {
            isChainValid = false;
        }
        if (area.empty())
        {
            isChainEmpty = true;
        }
    }

    return isChainValid;
}
