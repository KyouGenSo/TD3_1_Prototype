#pragma once
#include <memory>

#include "EnemyBase.h"

class Boss : public EnemyBase
{
public:
    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Finalize() override;

    void ImGui() override;

    bool GetIsValid() const { return isValid_; }

    void SetIsValid(bool isValid);

    void OnCollision(const Collision::Collider* _other) override;
    void OnCollisionTrigger(const Collision::Collider* _other) override;

private:
    Vector3 prePos_ = {}; // 前回の位置
    bool isValid_ = false; // ボスの有効無効
};

