#include "FollowCamera.h"

#include "imgui.h"
#include "Object3dBasic.h"

void FollowCamera::Initialize() {
    pCamera_ = Object3dBasic::GetInstance()->GetCamera();
    pCamera_->SetFarClip(1000);
}

void FollowCamera::Update() {
    if (!pTarget_) return;
    
    // direction
    Vector3 rotate = { rotationX_, pTarget_->rotate.y, 0.0f };
    Vector3 direction = {};
	Matrix4x4 rotation = Mat4x4::MakeRotateXYZ(rotate);
    direction = Mat4x4::TransFormNormal(rotation, shiftDirection_);

    // interpolation
    Vector3 targetPosition = {};
    targetPosition = targetPositionPre_ * (1.0f - factorLerp_) + (pTarget_->translate + targetPositionOffset_) * factorLerp_;

    pCamera_->SetTranslate(direction.Normalize() * offset_ + targetPosition);
    pCamera_->SetRotate(rotate);
    pCamera_->Update();

    targetPositionPre_ = targetPosition;
}

void FollowCamera::Finalize() {
}

void FollowCamera::ImGui() {
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
