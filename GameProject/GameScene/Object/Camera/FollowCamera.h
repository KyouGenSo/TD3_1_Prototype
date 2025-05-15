#pragma once
#include "Camera.h"

#include <Collision/Collider.h>
#include <Collision/CollisionManager.h>

#include <Object3d.h>

#include <memory>

class FollowCamera
{
    Camera* pCamera_ = nullptr;
    const Transform* pTarget_ = nullptr;

    std::unique_ptr<Collision::Ray> pRay_ = nullptr;

    // Parameters
    Vector3 targetPositionPre_ = {};
    Vector3 shiftDirection_ = { 0.04f, 0.121f, -0.992f };
    Vector3 targetPositionOffset_ = { 0.0f, 0.0f, 0.0f };
    float offset_ = 20.0f;
    float factorLerp_ = 0.2f;
    float rotationX_ = 0.1f;
    Collision::Manager* pCollisionManager_ = nullptr;

    // for debug
    std::unique_ptr<Object3d> pDebugObjectCamera_ = nullptr;
    std::unique_ptr<Object3d> pDebugObjectHitPoint_ = nullptr;

public:
    void Initialize();
    void Update();
    void Finalize();
    void Draw3D();
    void Draw2D();

    void SetTarget(const Transform* pTarget) { pTarget_ = pTarget; }

    void SetTranslate(const Vector3& translate) { pCamera_->SetTranslate(translate); }

    void ImGui();
};

