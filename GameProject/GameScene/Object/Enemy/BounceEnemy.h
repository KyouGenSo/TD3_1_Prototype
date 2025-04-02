#pragma once
#include <memory>

#include "Object3d.h"
#include "Camera.h"
#include "EnemyBase.h"
#include "GameScene/Object/Collision/Collider.h"

class BounceEnemy : public EnemyBase
{
public:
    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Finalize();

    void OnCollision(const Collider* pCollider) override;

    void Move();

    void AppearanceProduction();

    void SetTranslate(Vector3 translate) { transform_.translate = translate; }

    void SetIsAppearing(bool isAppearing) { isAppearing_ = true; }

    void SetAppearCounter(float appearCounter) { appearCounter_ = appearCounter; }

    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }


private:
    std::unique_ptr<Object3d> model_;

    Camera* pCamera_ = nullptr;

    std::unique_ptr<Collider> collider_;

    bool isAppearing_ = true;
    float appearCounter_ = 0.0f;

    float targetRotate = 2.0f;

    Object* pTarget_ = nullptr;

    float speed = 0.2f;

    float appearDuration = 60.0f;

    Vector3 defaultScale = { 1.0f,1.0f,1.0f };
    Vector3 defaultRotate = {};

    float bounceTime_ = 0.0f;

    float bounceHight_ = 5.0f;

    float standardHeight_ = 1.0f;

    //status
    uint16_t hp_ = 5;
};

