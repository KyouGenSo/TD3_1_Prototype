#include "FollowCamera.h"

#include "imgui.h"
#include "Object3dBasic.h"
#include <Type/Singleton.h>
#include <Utility/Adaptor.h>
#include <Type/ColliderType.h>
#include <Draw2D.h>
#include <GPUParticle.h>

void FollowCamera::Initialize()
{
    pCamera_ = std::make_unique<Camera>();

    Object3dBasic::GetInstance()->SetCamera(pCamera_.get());
    GPUParticle::GetInstance()->SetCamera(pCamera_.get());
    Draw2D::GetInstance()->SetCamera(pCamera_.get());

    pCamera_->SetFarClip(1000.0f);

    pCollisionManager_ = Singleton<Collision::Manager>::GetInstance();
    pRay_ = std::make_unique<Collision::Ray>();
    // プレイヤーとだけ当たらないようにしたい。
    pRay_
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::CAMERA))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->SetType(Collision::Type::Ray);

    // Debug
    pDebugObjectCamera_ = std::make_unique<Object3d>();
    pDebugObjectCamera_->Initialize();
    pDebugObjectCamera_->SetModel("box.gltf");
    pDebugObjectCamera_->SetScale({ 0.5f, 0.5f, 0.5f });

    pDebugObjectHitPoint_ = std::make_unique<Object3d>();
    pDebugObjectHitPoint_->Initialize();
    pDebugObjectHitPoint_->SetModel("box.gltf");
    pDebugObjectHitPoint_->SetScale({ 0.1f, 0.1f, 0.1f });
}

void FollowCamera::Update()
{
    if (!pTarget_) return;

    // direction_
    rotationX_ = pTarget_->rotate.x;
    Vector3 rotate = { rotationX_, pTarget_->rotate.y, 0.0f };
    Matrix4x4 rotation = Mat4x4::MakeRotateXYZ(rotate);
    Vector3 direction = Mat4x4::TransFormNormal(rotation, shiftDirection_);

    // interpolation
    Vector3 nextTargetPosition = targetPositionPre_ * (1.0f - factorLerp_) + (pTarget_->translate + targetPositionOffset_) * factorLerp_;

    Vector3 nextCameraPosition = direction.Normalize() * offset_ + nextTargetPosition;

    // ray collision
    pRay_->SetOrigin(Adaptor(pTarget_->translate));
    pRay_->SetDestination(Adaptor(nextCameraPosition));
    pRay_->SetLength((nextCameraPosition - pTarget_->translate).Length());

    auto hitData = pCollisionManager_->RayCast(pRay_.get());


    // あたっていたら
    auto otherCollider = pCollisionManager_->Get(hitData.uuid);
    if (otherCollider)
    {
        pCamera_->SetTranslate(Adaptor(hitData.hitPoint));
        pDebugObjectHitPoint_->SetTranslate(Adaptor(hitData.hitPoint));
    }
    else
    {
        pCamera_->SetTranslate(nextCameraPosition);
        pDebugObjectHitPoint_->SetTranslate(nextCameraPosition);
    }

    pCamera_->SetRotate(rotate);
    pCamera_->Update();

    targetPositionPre_ = nextTargetPosition;

    pDebugObjectCamera_->SetRotate(rotate);
    pDebugObjectCamera_->SetTranslate(nextCameraPosition);
    pDebugObjectCamera_->Update();

    pDebugObjectHitPoint_->SetRotate(rotate);
    pDebugObjectHitPoint_->Update();

}

void FollowCamera::Finalize()
{
}

void FollowCamera::Draw3D()
{
    pDebugObjectCamera_->Draw();
    pDebugObjectHitPoint_->Draw();
}

void FollowCamera::Draw2D()
{
    Draw2D::GetInstance()->DrawLine(
        Adaptor(pRay_->GetOrigin()),
        Adaptor(pRay_->GetOrigin() + pRay_->GetDirection() * pRay_->GetLength()), 
        { 1.0f, 0.0f, 0.0f, 1.0f }
    );
}

void FollowCamera::ImGui()
{
    Vector3 translate = pCamera_->GetTranslate();
    Vector3 rotate = pCamera_->GetRotate();

    bool shiftDir = false;

    if (ImGui::Begin("Camera"))
    {
        if (ImGui::TreeNode("Common"))
        {
            ImGui::DragFloat3("Position", &translate.x);
            ImGui::DragFloat3("Rotation", &rotate.x);
            ImGui::DragFloat("RotationX", &rotationX_, 0.01f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Follow"))
        {
            ImGui::DragFloat("Offset", &offset_, 0.1f);
            shiftDir = ImGui::DragFloat3("ShiftDirection", &shiftDirection_.x, 0.01f);
            ImGui::DragFloat3("TargetPositionOffset", &targetPositionOffset_.x, 0.1f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Linear Interpolation"))
        {
            ImGui::DragFloat("Factor", &factorLerp_, 0.01f);
            ImGui::TreePop();
        }
    }
    ImGui::End();


    // post process
    if (shiftDir)
    {
        shiftDirection_ = shiftDirection_.Normalize();
    }
}
