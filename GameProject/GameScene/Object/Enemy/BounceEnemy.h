#pragma once
#include <memory>

#include "EnemyBase.h"

class BounceEnemy : public EnemyBase
{
public:
    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Finalize() override;

    void Move();

    void AppearanceProduction();


private:
    Vector3 prePos_ = {};

    float targetRotate = 2.0f;

    float speed = 14.f;

    float appearDuration = 60.0f;

    Vector3 defaultScale = { 1.0f,1.0f,1.0f };
    Vector3 defaultRotate = {};

    float bounceTime_ = 0.0f;

    float bounceHight_ = 5.0f;

    float standardHeight_ = 1.0f;
};

