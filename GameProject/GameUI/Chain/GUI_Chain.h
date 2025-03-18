#pragma once

#include <Interfaces/IObserver.h>
#include <string>

class GUI_Chain : public IObserver
{
public:
    GUI_Chain() = default;
    ~GUI_Chain() = default;

    // 通知する
    void OnNotify(const std::string& _event) override;

public:
    void Update();
    void ImGui();

private:
    const std::string TEXTUREPATH_ = "white.png";
    bool isDisplay_ = false;
    std::string area1_ = "";
    std::string area2_ = "";
    std::string area3_ = "";

private:
    // チェイン画面を表示する
    void ShowChain();
};