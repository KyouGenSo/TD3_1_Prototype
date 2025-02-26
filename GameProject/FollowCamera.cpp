#include "FollowCamera.h"

#include "imgui.h"
#include "Input.h"
#include "Object3dBasic.h"

void FollowCamera::Initialize() {
    pCamera_ = Object3dBasic::GetInstance()->GetCamera();
}

void FollowCamera::Update() {
    //rotation
    if(Input::GetInstance()->IsConnect()){
	    //Joycon
    } else{
        yaw += static_cast<float>(Input::GetInstance()->PushKey(DIK_RIGHTARROW) - Input::GetInstance()->PushKey(DIK_LEFTARROW)) * 0.1f;
    }
    pCamera_->SetRotate({0.f, yaw, 0.f});

	//translate
    if (pTarget_){
	    Matrix4x4 rotation = Mat4x4::MakeRotateXYZ(pCamera_->GetRotate());
	    Vector3 offset = Mat4x4::TransFormNormal(rotation, offset_);
        pCamera_->SetTranslate(pTarget_->translate + offset);
    }
}

void FollowCamera::Finalize() {
}

void FollowCamera::ImGui() {
    Vector3 translate = pCamera_->GetTranslate();
    Vector3 rotate = pCamera_->GetRotate();
    ImGui::Begin("Camera");
    ImGui::Text("Position : ");
    ImGui::SameLine();
    ImGui::DragFloat3("##pos", &translate.x, 0.1f);
    ImGui::Text("Rotate   : ");
    ImGui::SameLine();
    ImGui::DragFloat3("##rotate", &rotate.x, 0.1f);
	ImGui::Text("Offset : ");
    ImGui::SameLine();
    ImGui::DragFloat3("##offset", &offset_.x, 0.1f);
    ImGui::End();
}
