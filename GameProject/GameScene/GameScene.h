#pragma once
#include <memory>

#include "BaseScene.h"
#include <GameUI/LvUP/GUI_LvUP.h>
#include <GameUI/PauseMenu/GUI_PauseMenu.h>

#include "HUD/Minimap.h"
#include "Object/Camera/FollowCamera.h"
#include "Object/Collision/CollisionManager.h"
#include "Object/Enemy/Boss.h"
#include "Object/Enemy/Enemy.h"
#include "Object/Player/Player.h"
#include "Object/Terrain/Terrain.h"
#include "Object/Enemy/EnemyManager.h"

class GameScene : public BaseScene {
    std::unique_ptr<Player> player_;
    std::unique_ptr<FollowCamera> camera_;
    std::unique_ptr<GUI_LvUP> guiLvUP_;
    std::unique_ptr<GUI_PauseMenu> guiPauseMenu_;
    std::unique_ptr<Minimap> minimap_; 
    CollisionManager* pCollisionManager_ = nullptr;
	std::unique_ptr<Enemy>enemy_;
	std::unique_ptr<Boss>boss_;
    std::unique_ptr<Terrain> terrain_;
	EnemyManager enemyManager_;

public:
    struct DirectionalLightParam {
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

