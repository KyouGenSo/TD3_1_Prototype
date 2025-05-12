#pragma once
#include "Sprite.h"
#include "GameScene/Object/Object.h"

class Player;

class Crosshair final : public Object{
    std::unique_ptr<Sprite> sprite_;

public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
};

