#define NOMINMAX
#include "Player.h"

#include "imgui.h"
#include "Input.h"

#include <ModelManager.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>
#include <GameScene/Object/Weapon/WeaponFactory.h>

// DEBUG
#include <QuatFunc.h>

#include "GameScene/Object/Weapon/SMG/SMG.h"
#include "Type/ColliderType.h"
#include <GameSystem/Reinforcement/StatusReinforcement.h>
#include <GameSystem/Reinforcement/Manager/ReinforcementManager.h>

#include <algorithm>
#include <Utility/Adaptor.h>

void Player::Initialize()
{
    Object::Initialize();

    ModelManager::GetInstance()->LoadModel("box.gltf");

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("box.gltf");

    // 初期化用 - 外部から設定するためこの値は適用されない
    transform_ = {
        .scale = { 1.0f, 1.0f, 1.0f },
        .rotate = { 0.0f, 0.0f, 0.0f },
        .translate = { 0.0f, 0.0f, 0.0f },
    };

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_->SetEvent(Collision::EventType::Stay, [this](const Collision::Collider* pCol){this->OnCollision(pCol); })
        ->SetEvent(Collision::EventType::Trigger, [this](const Collision::Collider* pCol) { this->OnCollisionTrigger(pCol); })
        ->SetTranslate(Adaptor(transform_.translate))
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::CAMERA))
        ->SetSize(1.f)
        ->SetOwner(this)
        ->Enable();

    // Status Initialize
    statusInit_
        .setAttack(0)
        .setHp(100)
        .setLevel(1)
        .setExp(0)
        .setMaxExp(100)
        .setSpeed(1)
        .setDefence(0)
        .setMaxHp(100);
    statusCurrent_ = statusInit_;

    /// !!Debug!!
    weapon_ = std::make_unique<SMG>();
    weapon_->Initialize();
    gravity_ = 1.8f;
}

void Player::Update()
{
    if (deltaTime_ == 0.0f)
    {
        for (auto observer : observers_)
        {
            observer->OnNotify("toggle_lvup");
        }
    }
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

    statusCurrent_.Update();
}

void Player::Draw()
{
    weapon_->Draw();
    model_->Update();
    model_->Draw();
}

void Player::Finalize()
{
    auto* rfmManager = ReinforcementManager::GetInstance();
    for (auto& reinforcement : reinforcementList_)
    {
        rfmManager->UnregisterReinforcement(reinforcement.get());
    }
    ShowCursor(true);
}

void Player::ImGui()
{
    statusInit_.ImGui("PlayerInit");
    statusCurrent_.ImGui("PlayerCurrent");

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

void Player::OnCollisionTrigger(const Collision::Collider* pCollider)
{
    Object::StatusUpdateOnCollision(pCollider);

    if (pCollider->GetAttribute() & static_cast<uint32_t>(Collider::Type::ENEMY))
    {
        //Collision::Vec3 pos = pCollider->GetTranslate();
        //Vector3 diff = transform_.translate - Vector3(pos.x, pos.y, pos.z);
        //diff.Normalize();
        //if (isGround_)
        //{
        //    ApplyForce(diff * 3000.0f);
        //}
        //else
        //{
        //    ApplyForce(diff);
        //}
        
    }
}

void Player::AddReinforcement(const std::string& _cardName)
{
    auto reinforcement = std::make_unique<StatusReinforcement>();
    reinforcement->Initialize(_cardName);
    reinforcement->SetStatus(&statusCurrent_);
    reinforcement->Apply();

    reinforcementList_.emplace_back(std::move(reinforcement));

    ReinforcementManager::GetInstance()->RegisterReinforcement(reinforcementList_.back().get());
}

void Player::OnChainConfirm()
{
    weapon_.reset();
    weapon_ = WeaponFactory::CreateWeapon(chain_->Front());
    weapon_->Initialize();
    weapon_->SetChain(chain_);
    weapon_->SetEmitter(emitter_);
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

    if (pInput_->TriggerKey(DIK_M)){
        mouseAim_ = !mouseAim_;
        ShowCursor(false);
    }

    // Perspective
    if (mouseAim_){
        POINT point;
        GetCursorPos(&point);
        SetCursorPos(ORIGIN.x, ORIGIN.y);

        transform_.rotate.y += static_cast<float>(point.x - ORIGIN.x) * 0.001f;
        transform_.rotate.x += static_cast<float>(point.y - ORIGIN.y) * 0.001f;
    } else{
        transform_.rotate.y += static_cast<float>(Input::GetInstance()->PushKey(DIK_RIGHTARROW) - Input::GetInstance()->PushKey(DIK_LEFTARROW)) * 0.03f;
        transform_.rotate.x += static_cast<float>(Input::GetInstance()->PushKey(DIK_UPARROW) - Input::GetInstance()->PushKey(DIK_DOWNARROW)) * 0.03f;
    }

}

void Player::UpdateMovement()
{
    // floor clamp
    transform_.translate.y = std::max(transform_.translate.y, floor_);

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

