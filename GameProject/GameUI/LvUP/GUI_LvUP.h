#pragma once

#include <string> // std::string

#include <Interfaces/IObserver.h>
#include <GameSystem/GameController/GameController.h>
#include <vector>
#include <GameSystem/Reinforcement/Manager/ReinforcementManager.h>

class GUI_LvUP : public IObserver
{
public:
    GUI_LvUP() = default;
    ~GUI_LvUP() override = default;

    // 通知する
    void OnNotify(const std::string& _event) override;
    void SetGameController(GameController* _gameController) { gameController_ = _gameController; }

public:
    void Update();
    void ImGui();

private:
    // レベルアップ画面を表示する
    void ShowLvUP();
    void PickCardName();


private:
    const std::string TEXTUREPATH_ = "hud/card/background.png";
    const std::string TEX_CARD_ATTACK_  = "hud/card/card_attack_shadow.png";
    const std::string TEX_CARD_DEFENCE_ = "hud/card/card_defence_shadow.png";
    const std::string TEX_CARD_STEEL_   = "hud/card/card_steel_shadow.png";

    std::vector<ReinforcementData> rfCards_ = {};

    // レベルアップ画面を表示するかどうか
    bool isDisplay_ = false;

    bool isSelected_ = false;
    std::string selectedCard_ = "none";

    bool isPickRandom_ = false;

private:
    GameController* gameController_ = nullptr;
};