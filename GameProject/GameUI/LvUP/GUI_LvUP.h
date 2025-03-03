#pragma once

#include <string> // std::string

#include <Interfaces/IObserver.h>

class GUI_LvUP : public IObserver
{
public:
    GUI_LvUP() = default;
    ~GUI_LvUP() = default;

    // 通知する
    void OnNotify(const std::string& _event) override;

public:
    void Update();

private:
    // レベルアップ画面を表示する
    void ShowLvUP();


private:
    // レベルアップ画面を表示するかどうか
    bool isDisplay_ = false;

    const std::string TEXTUREPATH_ = "white.png";
};