#pragma once
#include "BaseScene.h"
#include "CollisionManager.h"
#include "FollowCamera.h"
#include "Player.h"
#include <GameUI/LvUP/GUI_LvUP.h>
#include "Enemy.h"
#include "Boss.h"

class GameScene : public BaseScene {
    std::unique_ptr<Player> player_;
    std::unique_ptr<FollowCamera> camera_;
    std::unique_ptr<GUI_LvUP> guiLvUP_;
    CollisionManager* pCollisionManager_ = nullptr;
	std::unique_ptr<Enemy>enemy_;
	std::unique_ptr<Boss>boss_;


public:
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void DrawImGui() override;
};

