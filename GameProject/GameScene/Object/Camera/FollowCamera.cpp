#include "FollowCamera.h"

#include "imgui.h"
#include "Input.h"
#include "Object3dBasic.h"

void FollowCamera::Initialize() {
    pCamera_ = Object3dBasic::GetInstance()->GetCamera();
}

void FollowCamera::Update() {
    // rotation
    if(Input::GetInstance()->IsConnect()){
    // Joycon
    } else{
        yaw_ += static_cast<float>(Input::GetInstance()->PushKey(DIK_RIGHTARROW) - Input::GetInstance()->PushKey(DIK_LEFTARROW)) * 0.03f;
    }
    pCamera_->SetRotate(Vector3{ 0, yaw_, 0.f });
    
    // direction
    Vector3 direction = {};
    if (!pTarget_) return;
	Matrix4x4 rotation = Mat4x4::MakeRotateXYZ(pCamera_->GetRotate());
    direction = Mat4x4::TransFormNormal(rotation, shiftDirection_);

    // interpolation
    Vector3 targetPosition = {};
    targetPosition = targetPositionPre_ * (1.0f - factorLerp_) + pTarget_->translate * factorLerp_;

    pCamera_->SetTranslate(direction.Normalize() * offset_ + targetPosition);
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
            ImGui::DragFloat3("Position", &translate.x, 0.1f);
            ImGui::DragFloat3("##rotate", &rotate.x, 0.1f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Follow"))
        {
            ImGui::DragFloat("Offset", &offset_, 0.1f);
            ImGui::DragFloat("Yaw", &yaw_, 0.01f);
            shiftDir = ImGui::DragFloat3("ShiftDirection", &shiftDirection_.x, 0.01f);
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
