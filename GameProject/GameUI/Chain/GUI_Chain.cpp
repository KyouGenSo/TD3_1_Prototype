#include "GUI_Chain.h"

#include <NiGui.h>
#include <imgui.h>

#include <GameSystem/GameController/GameController.h>

void GUI_Chain::OnNotify(const std::string& _event)
{
    if (_event == "open_chain")
    {
        isDisplay_ = true;
    }
    else if (_event == "close_chain")
    {
        isDisplay_ = false;
    }
    else if (_event == "toggle_chain")
    {
        isDisplay_ = !isDisplay_;
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
    auto confirm = NiGui_ButtonState::Confirm;
    if (NiGui::BeginDiv("Chain", TEX_WHITE_, NiGui::WHITE, { 0, 0 }, { 800, 450 }, center, center))
    {
        area1_ = NiGui::DragItemArea("DragItemArea1", (FRAMEDIR_ / TEX_FRAME_).string(), NiGui::WHITE, { -240, 0 }, { 120, 120 }, center, center);
        area2_ = NiGui::DragItemArea("DragItemArea2", (FRAMEDIR_ / TEX_FRAME_).string(), NiGui::WHITE, { 0, 0 }, { 120, 120 }, center, center);
        area3_ = NiGui::DragItemArea("DragItemArea3", (FRAMEDIR_ / TEX_FRAME_).string(), NiGui::WHITE, { 240, 0 }, { 120, 120 }, center, center);
        NiGui::DragItem("RocketLauncher", (ICONDIR_ / TEX_ROCKETLAUNCHER_).string(), NiGui::WHITE, { -240, -150 }, { 100, 100 }, center, center);
        NiGui::DragItem("Assault", (ICONDIR_ / TEX_ASSAULT_).string(), NiGui::WHITE, { 0, -150 }, { 100, 100 }, center, center);
        NiGui::DragItem("MachineGun", TEX_WHITE_, NiGui::MAGENTA, { 240, -150 }, { 100, 100 }, center, center);

        if (NiGui::Button("ConfirmChain", TEX_WHITE_, NiGui::GREEN, { 0, 150 }, { 100, 100 }, center, center) == confirm)
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
