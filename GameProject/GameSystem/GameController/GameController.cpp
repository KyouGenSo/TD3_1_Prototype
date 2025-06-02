#include "GameController.h"

void GameController::OnNotify(const std::string& _event)
{
    if (_event == "level_up")
    {
        guiLvUP_->OnNotify("lvup", "open");
    }
    else if (_event == "open_chain")
    {
        guiChainView_->OnNotify("chain", "open");
    }
}
