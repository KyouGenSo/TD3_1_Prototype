#pragma once 

#include <GameScene/Object/Player/Player.h>
#include <Interfaces/IObserver.h>
#include <string>

// ビューからの入力を受け取り、モデルに反映する
class GameController
{
public:
    GameController() = default;

    void SetPlayerModel(Player* _playerModel) { playerModel_ = _playerModel; }
    void SetGUIChainView(IObserver* _guiChainView) { guiChainView_ = _guiChainView; }

    void OnNotify(const std::string& _event);

    /// <summary>
    /// (呼び出し元:UI) チェインが確定されたときの処理
    /// </summary>
    void HandleConfirmChain()
    {
        playerModel_->OnChainConfirm();
    }

    void HandleConfirmCard(const std::string& _cardName)
    {
        playerModel_->AddReinforcement(_cardName);
    }

    void OnLevelUp()
    {
        guiChainView_->OnNotify("chain", "open");
    }

private:
    Player* playerModel_ = nullptr;
    IObserver* guiChainView_ = nullptr;
    IObserver* guiLvUP_ = nullptr;
};