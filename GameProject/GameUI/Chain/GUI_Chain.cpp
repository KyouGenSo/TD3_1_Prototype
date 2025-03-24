#include "GUI_Chain.h"

#include <NiGui.h>
#include <imgui.h>

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
        chainViewModel_->UpdateChainData({ area1_, area2_, area3_, "" });
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
    auto confirm = NiGui_ButtonState::Confirm::Center;
    if (NiGui::BeginDiv("Chain", TEXTUREPATH_, NiGui::WHITE, { 0, 0 }, { 800, 450 }, center, center))
    {
        area1_ = NiGui::DragItemArea("DragItemArea1", TEXTUREPATH_, NiGui::BLUE, { -240, 150 }, { 120, 120 }, center, center);
        area2_ = NiGui::DragItemArea("DragItemArea2", TEXTUREPATH_, NiGui::BLUE, { 0, 150 }, { 120, 120 }, center, center);
        area3_ = NiGui::DragItemArea("DragItemArea3", TEXTUREPATH_, NiGui::BLUE, { 240, 150 }, { 120, 120 }, center, center);
        NiGui::DragItem("RocketLauncher", TEXTUREPATH_, NiGui::YELLOW, { -240, -150 }, { 100, 100 }, center, center);
        NiGui::DragItem("Assault", TEXTUREPATH_, NiGui::CIAN, { 0, -150 }, { 100, 100 }, center, center);
        NiGui::DragItem("MachineGun", TEXTUREPATH_, NiGui::MAGENTA, { 240, -150 }, { 100, 100 }, center, center);

        if (NiGui::Button("ConfirmChain", TEXTUREPATH_, NiGui::GREEN, { 0, 0 }, { 100, 100 }, center, center) == )
        {
            isConfirm_ = true;
        });
        NiGui::EndDiv();
    }
}
