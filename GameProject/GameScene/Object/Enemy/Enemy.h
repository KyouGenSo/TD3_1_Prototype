#pragma once
#include <memory>

#include "EnemyBase.h"
#include "GameScene/Object/Object.h"
#include "Collision/Collider.h"
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>

class Enemy : public EnemyBase
{
public:
    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Finalize() override;

	void OnCollision(const Collision::Collider* pCollider) override;

    void OnCollisionTrigger(const Collision::Collider* pCollider) override;

    void Move();

    void AppearanceProduction();

    float GetAmoRotate() { return amoRotate_; }

    Vector3 GetDirection() { return direction_; }


private:
    Vector3 prePos_ = {};

    float amoRotate_ = 0.1f;
    float targetRotate_ = 2.0f;
    float rotateSpeed_ = 0.1f;
    float preRotateSpeed_ = 0.1f;

    Vector3 direction_{ 0.0f,0.0f,0.0f };

    float speed_ = 0.2f;

    float appearDuration_ = 60.0f;

    Vector3 defaultScale_ = { 1.0f,1.0f,1.0f };
    Vector3 defaultRotate_ = {};
};

