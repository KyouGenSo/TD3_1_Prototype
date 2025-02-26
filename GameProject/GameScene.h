#pragma once
#include "BaseScene.h"

class GameScene : public BaseScene{
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DrawImGui() override;
};

