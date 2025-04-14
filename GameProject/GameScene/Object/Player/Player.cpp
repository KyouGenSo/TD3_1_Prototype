#include "Player.h"

#include "imgui.h"
#include "Input.h"

#include <ModelManager.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>
#include <GameScene/Object/Weapon/WeaponFactory.h>

// DEBUG
#include <QuatFunc.h>

#include "GameScene/Object/Weapon/AssaultRifle/AssaultRifle.h"
#include "GameScene/Object/Weapon/SMG/SMG.h"
#include "Type/ColliderType.h"

void Player::Initialize()
{
    Object::Initialize();

    ModelManager::GetInstance()->LoadModel("box.gltf");

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetCamera(pCamera_);
    model_->SetModel("box.gltf");

    // 初期化用 - 外部から設定するためこの値は適用されない
    transform_ = {
        .scale = { 1.0f, 1.0f, 1.0f },
        .rotate = { 0.0f, 0.0f, 0.0f },
        .translate = { 0.0f, 0.0f, 0.0f },
    };

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_->SetEvent(Collision::EventType::Stay, [this](const Collision::Collider* pCol){this->OnCollision(pCol); })
        ->SetTranslate(Adaptor(transform_.translate))
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ALLY))
        ->SetSize(1.f)
        ->Enable();

    // Status Initialize
    status_
        .setAttack(0)
        .setHp(100)
        .setLevel(1)
        .setExp(0)
        .setMaxExp(100)
        .setSpeed(1)
        .setDefence(0)
        .setMaxHp(100);

    /// !!Debug!!
    weapon_ = std::make_unique<SMG>();
    gravity_ = 1.8f;
}

void Player::Update()
{
    deltaTime_ = DeltaTimeManager::GetInstance()->GetDeltaTime(0);

    UpdateInputCommands();
    UpdateMovement();

    weapon_->SetPosition(transform_.translate);
    weapon_->SetRotation(transform_.rotate);
    weapon_->Update();

    pCollider_->SetTranslate(Adaptor(transform_.translate));

    /// Model Update
    model_->SetScale(transform_.scale);
    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
}

void Player::Draw()
{
    weapon_->Draw();
    model_->Update();
    model_->Draw();
}

void Player::Finalize()
{
}

void Player::ImGui()
{

    if (ImGui::Begin("Player"))
    {
        if (ImGui::TreeNode("Object"))
        {
            Object::DebugObject();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Common"))
        {
            ImGui::DragFloat("JumpPower", &jumpPower_, 0.01f);
            ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);
            ImGui::DragFloat("FrictionCoefficient", &frictionCoefficient_, 0.01f);
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void Player::OnCollision(const Collision::Collider* pCollider) {
}

void Player::OnChainConfirm()
{
    weapon_.reset();
    weapon_ = WeaponFactory::CreateWeapon(chain_->Front());
    weapon_->SetChain(chain_);
}

void Player::UpdateInputCommands()
{
    // Attack
    if (pInput_->PushKey(DIK_RETURN) || pInput_->PushButton(JOY_BUTTON1))
    {
        weapon_->Fire();
    }

    if (pInput_->TriggerKey(DIK_ESCAPE))
    {
        for (auto observer : observers_)
        {
            observer->OnNotify("toggle_pause_menu");
        }
    }
    if (pInput_->TriggerKey(DIK_TAB))
    {
        for (auto observer : observers_)
        {
            observer->OnNotify("toggle_lvup");
        }
    }
    if (pInput_->TriggerKey(DIK_C))
    {
        for (auto observer : observers_)
        {
            observer->OnNotify("toggle_chain");
        }
    }

    // Perspective
    transform_.rotate.y += static_cast<float>(Input::GetInstance()->PushKey(DIK_RIGHTARROW) - Input::GetInstance()->PushKey(DIK_LEFTARROW)) * 0.03f;
    transform_.rotate.x += static_cast<float>(Input::GetInstance()->PushKey(DIK_UPARROW) - Input::GetInstance()->PushKey(DIK_DOWNARROW)) * 0.03f;
}

void Player::UpdateMovement()
{
    // floor clamp
    if (transform_.translate.y < floor_)
    {
        transform_.translate.y = floor_;
    }

    // Movement
    if (Input::GetInstance()->IsConnect())
    {
        // Joycon Movement
    }
    else
    {
        Quaternion yaw = Quat::MakeRotateAxisAngle({ 0.0f, 1.0f, 0.0f }, transform_.rotate.y);
        Quaternion pitch = Quat::MakeRotateAxisAngle({ 1.0f, 0.0f, 0.0f }, 0.0f);

        Quaternion rotate = yaw * pitch;

        Vector3 forward_ = Quat::RotateVec3({ 0.0f, 0.0f, 1.0f }, rotate);
        Vector3 right = Quat::RotateVec3({ 1.0f, 0.0f, 0.0f }, rotate);

        int directionForward = Input::GetInstance()->PushKey(DIK_W) - Input::GetInstance()->PushKey(DIK_S);
        int directionRight = Input::GetInstance()->PushKey(DIK_D) - Input::GetInstance()->PushKey(DIK_A);
        acceleration_ += forward_ * moveSpeed_ * static_cast<float>(directionForward) * deltaTime_;
        acceleration_ += right * moveSpeed_ * static_cast<float>(directionRight) * deltaTime_;
    }


    // Jump
    if (isGround_)
    {
        if (Input::GetInstance()->TriggerKey(DIK_SPACE))
        {
            acceleration_.y += jumpPower_;
            isGround_ = false;
        }
        ApplyFriction(frictionCoefficient_);
    }
    else
    {
        // 重力を加算
        acceleration_.y += -gravity_;
    }

    // 速度を加算
    velocity_ += acceleration_;


    transform_.translate += velocity_ * deltaTime_;

    if (transform_.translate.y < floor_ + HEIGHT_HALF)
    {
        transform_.translate.y = floor_ + HEIGHT_HALF;
        isGround_ = true;
        velocity_.y = 0.0f;
    }

    // 加速度初期化
    acceleration_ = {};
}

