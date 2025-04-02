#pragma once
#include "GameScene/Object/Object.h"

class Castle : public Object{
    std::unique_ptr<Collider> collider_;
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
};

