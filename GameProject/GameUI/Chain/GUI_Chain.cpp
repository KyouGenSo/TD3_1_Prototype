#include "GUI_Chain.h"

#include <NiUI/NiUI.h>
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
    auto center = NiUI_StandardPoint::Center;
    if (NiUI::BeginDiv("Chain", TEXTUREPATH_, NiUI::WHITE, { 0, 0 }, { 800, 450 }, center, center))
    {
        area1_ = NiUI::DragItemArea("DragItemArea1", TEXTUREPATH_, NiUI::BLUE, { -240, 150 }, { 120, 120 }, center, center);
        area2_ = NiUI::DragItemArea("DragItemArea2", TEXTUREPATH_, NiUI::BLUE, { 0, 150 }, { 120, 120 }, center, center);
        area3_ = NiUI::DragItemArea("DragItemArea3", TEXTUREPATH_, NiUI::BLUE, { 240, 150 }, { 120, 120 }, center, center);
        NiUI::DragItem("Yellow", TEXTUREPATH_, NiUI::YELLOW, { -240, -150 }, { 100, 100 }, center, center);
        NiUI::DragItem("Cian", TEXTUREPATH_, NiUI::CIAN, { 0, -150 }, { 100, 100 }, center, center);
        NiUI::DragItem("Magenta", TEXTUREPATH_, NiUI::MAGENTA, { 240, -150 }, { 100, 100 }, center, center);
        NiUI::EndDiv();
    }
}
