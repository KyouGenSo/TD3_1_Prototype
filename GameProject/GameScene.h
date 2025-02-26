#pragma once
#include "BaseScene.h"
#include "FollowCamera.h"
#include "Player.h"

class GameScene : public BaseScene{
	std::unique_ptr<Player> player_;
	std::unique_ptr<FollowCamera> camera_;
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DrawImGui() override;
};

