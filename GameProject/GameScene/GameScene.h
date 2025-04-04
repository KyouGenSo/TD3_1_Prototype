#pragma once
#include <memory>

#include "BaseScene.h"
#include <GameUI/LvUP/GUI_LvUP.h>
#include <GameUI/PauseMenu/GUI_PauseMenu.h>
#include <GameUI/Chain/GUI_Chain.h>

#include "HUD/Minimap.h"
#include "Object/Camera/FollowCamera.h"
#include "Object/Castle/Castle.h"
#include "Object/Collision/CollisionManager.h"
#include "Object/Enemy/Boss.h"
#include "Object/Player/Player.h"
#include "Object/Terrain/Terrain.h"
#include "Object/Enemy/EnemyManager.h"
#include <EventTimer/EventTimer.h>
#include <GameSystem/TimeKeeper/TimeKeeper.h>
#include <GameSystem/ChainViewModel/ChainViewModel.h>
#include <GameSystem/GameController/GameController.h>
#include <Utility/Threadpool/Threadpool.h>
#include <GameUI/CountDown/CountDown.h>

class GameScene : public BaseScene
{
    std::unique_ptr<Player> player_;
    std::unique_ptr<FollowCamera> camera_;
    std::unique_ptr<GUI_LvUP> guiLvUP_;
    std::unique_ptr<GUI_PauseMenu> guiPauseMenu_;
    std::unique_ptr<GUI_Chain> guiChain_;
    std::unique_ptr<GameController> gameController_;

    std::unique_ptr<ChainViewModel> chainViewModel_;

    std::unique_ptr<Minimap> minimap_;
    CollisionManager* pCollisionManager_ = nullptr;
    std::unique_ptr<Boss>boss_;
    std::unique_ptr<Terrain> terrain_;
    std::unique_ptr<EnemyManager> enemyManager_;

    std::unique_ptr<Castle> castle_;
    EventTimer* eventTimer_ = nullptr;
    Threadpool* threadpool_ = nullptr;
    std::unique_ptr<TimeKeeper> timeKeeper_;
    std::unique_ptr<CountDown> countDown_;

public:
    struct DirectionalLightParam
    {
        Vector3 direction;
        Vector4 color;
        int32_t lightType;
        float intensity;
    };

    DirectionalLightParam directLightParam_;

public:
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void DrawImGui() override;
};

