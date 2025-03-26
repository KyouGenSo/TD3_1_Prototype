#pragma once

#include <Interfaces/IObserver.h>
#include <string>
#include <GameSystem/ChainViewModel/ChainViewModel.h>
#include <filesystem>

class GameController;

class GUI_Chain : public IObserver
{
public:
    GUI_Chain() = default;
    ~GUI_Chain() = default;

    // 通知する
    void OnNotify(const std::string& _event) override;
    void SetViewModel(ChainViewModel* _viewModel) { chainViewModel_ = _viewModel; }
    void SetGameController(GameController* _gameController) { gameController_ = _gameController; }

public:
    void Update();
    void ImGui();

private:
    const std::string TEX_WHITE_ = "white.png";
    const std::filesystem::path ICONDIR_ = "ico";
    const std::filesystem::path FRAMEDIR_ = "frame";
    const std::filesystem::path TEX_ROCKETLAUNCHER_ = "RocketLauncher_WhiteBase.png";
    const std::filesystem::path TEX_FRAME_ = "Simple.png";
    bool isDisplay_ = false;
    bool isConfirm_ = false;
    std::string area1_ = "";
    std::string area2_ = "";
    std::string area3_ = "";

private:
    // チェイン画面を表示する
    void ShowChain();

private:
    ChainViewModel* chainViewModel_;
    GameController* gameController_;
};