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
    ~GUI_Chain() override = default;

    void Initialize();

    // 通知する
    void OnNotify(const std::string& _event) override;
    void SetViewModel(ChainViewModel* _viewModel) { chainViewModel_ = _viewModel; }
    void SetGameController(GameController* _gameController) { gameController_ = _gameController; }

public:
    void Update();
    void ImGui();

private:
    const std::string TEX_WHITE_ = "white.png";
    const std::filesystem::path ICONDIR_ = "hud/chain/ico";
    const std::filesystem::path CHAINDIR_ = "hud/chain";
    const std::filesystem::path TEX_ROCKETLAUNCHER_ = "RocketLauncher_Text.png";
    const std::filesystem::path TEX_ASSAULT_ = "Assault_Text.png";
    const std::filesystem::path TEX_FRAME_ = "itemarea.png";
    const std::filesystem::path TEX_WND_ = "window.png";
    const std::filesystem::path TEX_BUTTON_CONFIRM_ = "button_confirm.png";
    const std::filesystem::path TEX_BUTTON_CANCEL_ = "button_cancel.png";
    const std::filesystem::path TEX_HOLDER_ = "holder.png";
    bool isDisplay_ = false;
    bool isConfirm_ = false;
    std::string area1_ = "";
    std::string area2_ = "";
    std::string area3_ = "";

private:
    // チェイン画面を表示する
    void ShowChain();
    // チェインが正しいか確認する 正:true, 誤: false
    bool CheckValidChain();

private:
    ChainViewModel* chainViewModel_ = nullptr;
    GameController* gameController_ = nullptr;
};