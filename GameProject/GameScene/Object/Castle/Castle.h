#pragma once
#include "GameScene/Object/Object.h"

class Castle : public Object{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void OnCollisionTrigger(const Collision::Collider* pCollider) override;
    void ImGui();

};

