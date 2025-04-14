#pragma once

#include <string> // std::string

#include <Interfaces/IObserver.h>

class GUI_LvUP : public IObserver
{
public:
    GUI_LvUP() = default;
    ~GUI_LvUP() override = default;

    // 通知する
    void OnNotify(const std::string& _event) override;

public:
    void Update();
    void ImGui();

private:
    // レベルアップ画面を表示する
    void ShowLvUP();


private:
    const std::string TEXTUREPATH_ = "white.png";

    // レベルアップ画面を表示するかどうか
    bool isDisplay_ = false;

    std::string area1_ = "";
    std::string area2_ = "";
    std::string area3_ = "";
};