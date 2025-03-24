#pragma once 

#include <GameScene/Object/Player/Player.h>
#include <GameUI/Chain/GUI_Chain.h>

class GameController
{
public:
    void SetPlayerModel(Player* _playerModel) { playerModel_ = _playerModel; }
    void SetGUIChainView(GUI_Chain* _guiChainView) { guiChainView_ = _guiChainView; }

    /// <summary>
    /// (呼び出し元:UI) チェインが確定されたときの処理
    /// </summary>
    void HandleConfirmChain()
    {
    }

    void OnLevelUp()
    {
        guiChainView_->OnNotify("open_chain");
    }

private:
    Player* playerModel_;
    GUI_Chain* guiChainView_;

};