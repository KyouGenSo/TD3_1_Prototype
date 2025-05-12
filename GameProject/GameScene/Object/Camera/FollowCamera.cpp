#include "FollowCamera.h"

#include "imgui.h"
#include "Object3dBasic.h"
#include <Type/Singleton.h>
#include <Utility/Adaptor.h>
#include <Type/ColliderType.h>
#include <Draw2D.h>

void FollowCamera::Initialize()
{
    pCamera_ = Object3dBasic::GetInstance()->GetCamera();
    pCamera_->SetFarClip(1000.0f);

    pCollisionManager_ = Singleton<Collision::Manager>::GetInstance();
    pRay_ = std::make_unique<Collision::Ray>();
    pRay_->SetLength(100.0f);
    // プレイヤーとだけ当たらないようにしたい。
}

void FollowCamera::Update()
{
    if (!pTarget_) return;

    // direction_
    Vector3 rotate = { rotationX_, pTarget_->rotate.y, 0.0f };
    Matrix4x4 rotation = Mat4x4::MakeRotateXYZ(rotate);
    Vector3 direction = Mat4x4::TransFormNormal(rotation, shiftDirection_);

    // interpolation
    Vector3 targetPosition = targetPositionPre_ * (1.0f - factorLerp_) + (pTarget_->translate + targetPositionOffset_) * factorLerp_;

    pCamera_->SetRotate(rotate);
    pCamera_->SetTranslate(direction.Normalize() * offset_ + targetPosition);
    pCamera_->Update();

    targetPositionPre_ = targetPosition;
}

void FollowCamera::Finalize()
{
}

void FollowCamera::Draw2D()
{
    //Draw2D::GetInstance()->DrawLine(
    //    Adaptor(pRay_->GetOrigin()),
    //    Adaptor(pRay_->GetOrigin() + pRay_->GetDirection() * pRay_->GetLength()), 
    //    { 1.0f, 0.0f, 0.0f, 1.0f }
    //);
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
