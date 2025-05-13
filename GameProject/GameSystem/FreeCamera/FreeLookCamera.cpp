#include "FreeLookCamera.h"

#include <Math/NiVec3.h>
#include <Utility/Adaptor.h>

FreeLookCamera::FreeLookCamera()
{
    pInput_ = Input::GetInstance();
}

FreeLookCamera::~FreeLookCamera()
{
}

void FreeLookCamera::Initialize()
{
    pCamera_ = std::make_unique<Camera>();
    pCamera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
    pCamera_->SetRotate({ 0.0f, 0.0f, 0.0f });
}

void FreeLookCamera::Update()
{
    prevCursorPos_ = currCursorPos_;
    GetCursorPos(&currCursorPos_);

    if(pInput_->PushMouse(1))
    {
        this->CatchMoveCommands();
        this->CatchRotateCommands();
    }

    pCamera_->Update();
}

void FreeLookCamera::CatchMoveCommands()
{
    NiVec3 rotate = NiUtil::Adaptor(pCamera_->GetRotate());
    NiQuaternion yaw = NiQuaternion::RotateAxisAngleQuaternion({ 0.0f, 1.0f, 0.0f }, rotate.y);
    NiQuaternion pitch = NiQuaternion::RotateAxisAngleQuaternion({ 1.0f, 0.0f, 0.0f }, rotate.x);

    rotate_ = yaw * pitch;

    NiVec3 forward = FMath::RotateVector({ 0.0f, 0.0f, 1.0f }, rotate_);
    NiVec3 right = FMath::RotateVector({ 1.0f, 0.0f, 0.0f }, rotate_);

    /// 向いている方向に移動
    if(pInput_->PushKey(DIK_W))
    {
        NiVec3 cameraPosition = NiUtil::Adaptor(pCamera_->GetTranslate()) + forward * moveSpeed_;
        pCamera_->SetTranslate(NiUtil::Adaptor(cameraPosition));
    }
    if(pInput_->PushKey(DIK_S))
    {
        NiVec3 cameraPosition = NiUtil::Adaptor(pCamera_->GetTranslate()) - forward * moveSpeed_;
        pCamera_->SetTranslate(NiUtil::Adaptor(cameraPosition));
    }
    if(pInput_->PushKey(DIK_A))
    {
        NiVec3 cameraPosition = NiUtil::Adaptor(pCamera_->GetTranslate()) - right * moveSpeed_;
        pCamera_->SetTranslate(NiUtil::Adaptor(cameraPosition ));
    }
    if(pInput_->PushKey(DIK_D))
    {
        NiVec3 cameraPosition = NiUtil::Adaptor(pCamera_->GetTranslate()) + right * moveSpeed_;
        pCamera_->SetTranslate(NiUtil::Adaptor(cameraPosition));
    }
    if(pInput_->PushKey(DIK_Q))
    {
        NiVec3 cameraPosition = NiUtil::Adaptor(pCamera_->GetTranslate()) + NiVec3(0.0f, -moveSpeed_, 0.0f);
        pCamera_->SetTranslate(NiUtil::Adaptor(cameraPosition));
    }
    if(pInput_->PushKey(DIK_E))
    {
        NiVec3 cameraPosition = NiUtil::Adaptor(pCamera_->GetTranslate()) + NiVec3(0.0f, moveSpeed_, 0.0f);
        pCamera_->SetTranslate(NiUtil::Adaptor(cameraPosition));
    }
}

void FreeLookCamera::CatchRotateCommands()
{
    Vector2 diff = { 
        static_cast<float>(currCursorPos_.x - prevCursorPos_.x), 
        static_cast<float>(currCursorPos_.y - prevCursorPos_.y) 
    };

    Transform transform = pCamera_->GetTransform();
    transform.rotate.x += diff.y * 0.003f;
    transform.rotate.y += diff.x * 0.003f;

    pCamera_->SetRotate(transform.rotate);
    pCamera_->SetTranslate(transform.translate);
}
