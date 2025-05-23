#pragma once

#include <string> // std::string

#include <Interfaces/IObserver.h>
#include <GameSystem/GameController/GameController.h>
#include <vector>
#include <GameSystem/Reinforcement/Manager/ReinforcementManager.h>
#include <Type/NiGui_Type_Argument.h>
#include <GameUI/ColorResolver/ColorResolver.h>
#include <Vector2.h>

class GUI_LvUP : public IObserver
{
public:
    GUI_LvUP() = default;
    ~GUI_LvUP() override = default;

    // 通知する
    void Initialize();
    void OnNotify(const std::string& _event) override;
    void SetGameController(GameController* _gameController) { gameController_ = _gameController; }

    void OnResize(Vector2 _size);

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

    NiGui_Arg_Div arg_div_background_ = {};
    NiGui_Arg_Div arg_div_window_ = {};
    NiGui_Arg_Button arg_button_left_ = {};
    NiGui_Arg_Button arg_button_center_ = {};
    NiGui_Arg_Button arg_button_right_ = {};

    std::vector<ReinforcementData> rfCards_ = {};

    // レベルアップ画面を表示するかどうか
    bool isDisplay_ = false;

    bool isSelected_ = false;
    std::string selectedCard_ = "none";

    bool isPickRandom_ = false;

private:
    GameController* gameController_ = nullptr;
    ColorResolver* colorResolver_ = nullptr;
};