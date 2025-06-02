#define NOMINMAX
#include "Player.h"

#include "imgui.h"
#include "Input.h"

#include "Collision/Mathematics.h"
#include <Utility/Adaptor.h>

#include <ModelManager.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>
#include <GameScene/Object/Weapon/WeaponFactory.h>
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>

// DEBUG
#include <QuatFunc.h>

#include "Type/ColliderType.h"
#include <GameSystem/Reinforcement/StatusReinforcement.h>
#include <GameSystem/Reinforcement/Manager/ReinforcementManager.h>
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>

#include <algorithm>
#include <Utility/Adaptor.h>
#include <cmath>
#include <any>

#include "GameScene/Object/Weapon/AssaultRifle/AssaultRifle.h"
#include "GameScene/Object/Weapon/ThunderRod/ThunderRod.hpp"

void Player::Initialize()
{
    Object::Initialize();

    name_ = "Player";

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("player.gltf");

    // 初期化用 - 外部から設定するためこの値は適用されない
    transform_ = {
        .scale = { 1.0f, 1.0f, 1.0f },
        .rotate = { 0.0f, 0.0f, 0.0f },
        .translate = { 0.0f, 0.0f, 0.0f },
    };

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_->SetEvent(Collision::EventType::Stay, [this](const Collision::Collider* pCol) { this->OnCollision(pCol); })
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
        .setSpeed(1)
        .setDefence(0)
        .setMaxHp(100);
    statusCurrent_ = statusInit_;

    /// !!Debug!!
    weapon_ = std::make_unique<ThunderRod>();
    weapon_->Initialize();
    weapon_->SetChain(chain_);
    weapon_->SetEmitter(emitter_);
    gravity_ = 1.3f;

    this->InitializeCallbacks();

    GameEventNotifier::GetInstance()->RegisterCallback("OnWindowOpen", [this](std::any _isOpen)
    {
        ChangeAimMode(!std::any_cast<bool>(_isOpen));
    });
    ChangeAimMode(true); // 初期状態はマウスエイム
}

void Player::Update()
{
    deltaTime_ = DeltaTimeManager::GetInstance()->GetDeltaTime(1);

    isCollideCastle_ = false;

    UpdateInputCommands();
    UpdateMovement();
    UpdateOpacityByCameraDistance();

    weapon_->SetPosition(transform_.translate);
    weapon_->SetRotation(transform_.rotate);
    weapon_->Update();

    pCollider_->SetTranslate(Adaptor(transform_.translate));

    /// Model Update
    model_->SetScale(transform_.scale);
    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();

    statusCurrent_.Update();

    UpdateStatus();
}

void Player::Draw()
{
    weapon_->Draw();
    model_->Update();
    model_->Draw();
}

void Player::Finalize()
{
    GameEventNotifier::GetInstance()->UnregisterCallback("EnemyDeadForXP", id_callback_enemydead_);
    GameEventNotifier::GetInstance()->UnregisterCallback("PlayerLevelUp", id_callback_playerlevelup_);
    GameEventNotifier::GetInstance()->UnregisterCallback("ChainConfirm", id_callback_chainconfirm_);
    GameEventNotifier::GetInstance()->UnregisterCallback("ChainConfirm", id_callback_windowOpen_);

    auto* rfmManager = ReinforcementManager::GetInstance();
    for (auto& reinforcement : reinforcementList_)
    {
        rfmManager->UnregisterReinforcement(reinforcement.get());
    }

    if (mouseAim_)
    {
        ChangeAimModeForce(false);
    }
}

void Player::ImGui()
{
    Object::ImGui([&]() {
        statusInit_.ImGui("PlayerInit");
        statusCurrent_.ImGui("PlayerCurrent");

        if (ImGui::TreeNode("Common"))
        {
            ImGui::DragFloat("Start lerp distance", &distance_start_lerp_, 0.01f);
            ImGui::DragFloat("JumpPower", &jumpPower_, 0.01f);
            ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);
            ImGui::DragFloat("FrictionCoefficient", &frictionCoefficient_, 0.01f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Status"))
        {
            ImGui::Text("XP : %.1f", xp_);
            ImGui::Text("XP Max : %.1f", xpMax_);
            ImGui::ProgressBar(xp_ / xpMax_);
            ImGui::Text("Level : %.1f", level_);
            ImGui::TreePop();
        }
    });
}

void Player::OnCollision(const Collision::Collider* pCollider) {
    if (static_cast<Object*>(pCollider->GetOwner())->GetName() == "Castle") {
        isCollideCastle_ = true;

        // AABBの情報取得
        Vector3 colliderPos = {
            pCollider->GetTranslate().x,
            pCollider->GetTranslate().y,
            pCollider->GetTranslate().z
        };
        Vector3 colliderSize = Adaptor(std::get<Collision::Vec3>(pCollider->GetSize()));
        Vector3 aabbMin = colliderPos - colliderSize * 0.5f;
        Vector3 aabbMax = colliderPos + colliderSize * 0.5f;

        // XZ平面での最近点を計算（Y座標はPlayerの位置を使用）
        Vector3 closestPoint;
        closestPoint.x = std::clamp(transform_.translate.x, aabbMin.x, aabbMax.x);
        closestPoint.y = transform_.translate.y; // Y座標は変更しない
        closestPoint.z = std::clamp(transform_.translate.z, aabbMin.z, aabbMax.z);

        // XZ平面での押し出し方向
        Vector3 pushDirection;
        pushDirection.x = transform_.translate.x - closestPoint.x;
        pushDirection.y = 0.0f; // Y方向の押し出しなし
        pushDirection.z = transform_.translate.z - closestPoint.z;

        float distance = pushDirection.Length();

        if (distance > 0.0f) {
            pushDirection = pushDirection.Normalize();

            // XZ平面でのめり込み量計算
            float penetration = 1.f - distance;
            if (penetration > 0.0f) {
                // XZ平面でのみ位置補正
                transform_.translate.x += pushDirection.x * penetration;
                transform_.translate.z += pushDirection.z * penetration;
            }

            // 速度のXZ成分のみ反射
            Vector3 velocityXZ = { velocity_.x, 0.0f, velocity_.z };
            float dotProduct = Vec3::Dot(velocityXZ, pushDirection);

            if (dotProduct < 0.0f) {
                velocity_.x -= pushDirection.x * (dotProduct * 1.8f);
                velocity_.z -= pushDirection.z * (dotProduct * 1.8f);
            }
        }
    }
}

void Player::OnCollisionTrigger(const Collision::Collider* pCollider)
{
    if (static_cast<Object*>(pCollider->GetOwner())->GetName() != "Castle")
    {
        Object::StatusUpdateOnCollision(pCollider);
    }
}

void Player::AddReinforcement(const std::string& _cardName)
{
    auto reinforcement = std::make_unique<StatusReinforcement>();
    reinforcement->Initialize(_cardName);
    reinforcement->SetStatus(&statusCurrent_);
    reinforcement->SetBehaviorLimitter(&behaviorData_);
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

void Player::InitializeCallbacks()
{
    // コールバック登録
    id_callback_enemydead_ = GameEventNotifier::GetInstance()->RegisterCallback("EnemyDeadForXP", [this](std::any _gainedXP) {
        xpGained_ += std::any_cast<float>(_gainedXP);
    });

    id_callback_playerlevelup_ = GameEventNotifier::GetInstance()->RegisterCallback("PlayerLevelUp", [this]([[maybe_unused]]std::any _unused) {
        for (auto& obs : observers_)
        {
            obs->OnNotify("lvup", "toggle");
        }
    });

    id_callback_chainconfirm_ = GameEventNotifier::GetInstance()->RegisterCallback("ChainConfirm", [this]([[maybe_unused]]std::any _unused) {
        this->OnChainConfirm();
    });
}

void Player::UpdateInputCommands()
{
    // Attack
    if (pInput_->PushMouse(0) || pInput_->PushButton(JOY_BUTTON1)){
        if (weapon_){
            weapon_->Fire();
        }
    }

    if (pInput_->TriggerKey(DIK_M)){
        ToggleAimModeForce();
    }

    // Perspective
    if (mouseAim_){
        POINT point;
        GetCursorPos(&point);
        SetCursorPos(ORIGIN.x, ORIGIN.y);

        transform_.rotate.y += static_cast<float>(point.x - ORIGIN.x) * 0.001f * sensitivity_;
        transform_.rotate.x += static_cast<float>(point.y - ORIGIN.y) * 0.001f * sensitivity_;
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
    else if (!isCollideCastle_)
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
    if (numAbleJump_.get_current() > 0)
    {
        if (Input::GetInstance()->TriggerKey(DIK_SPACE))
        {
            acceleration_.y += jumpPower_ * (numAbleJump_.get_initial() - numAbleJump_.get_current() + 1);
            isGround_ = false;
            --numAbleJump_;
        }
    }

    if (isGround_)
    {
        ApplyFriction(frictionCoefficient_);
        numAbleJump_.reset(); // 地面にいる場合はジャンプ可能回数をリセット
    }
    else
    {
        // 重力を加算
        // 落下中かつスペースが押されていたらスロー
        if (velocity_.y < 0 && pInput_->PushKey(DIK_SPACE))
        {

        }
        acceleration_.y += -gravity_;
    }

    // 速度を加算
    velocity_ += acceleration_;
    transform_.translate += velocity_ * deltaTime_;

    if (transform_.translate.x > posXMinMax.max)
    {
        transform_.translate.x = posXMinMax.max;
        velocity_.x = 0.0f;
    }
    else if (transform_.translate.x < posXMinMax.min)
    {
        transform_.translate.x = posXMinMax.min;
        velocity_.x = 0.0f;
    }

    if (transform_.translate.z > posZMinMax.max)
    {
        transform_.translate.z = posZMinMax.max;
        velocity_.z = 0.0f;
    }
    else if (transform_.translate.z < posZMinMax.min)
    {
        transform_.translate.z = posZMinMax.min;
        velocity_.z = 0.0f;
    }

    if (transform_.translate.y < floor_ + HEIGHT_HALF)
    {
        transform_.translate.y = floor_ + HEIGHT_HALF;
        isGround_ = true;
        velocity_.y = 0.0f;
    }

    // 加速度初期化
    acceleration_ = {};
}

void Player::UpdateStatus()
{
    float xp_gained_actually = 0.0f;
    float xp_target = xp_ + xpGained_;
    float xp_pre = xp_;

    // 線形補間
    xp_ = std::lerp(xp_, xp_target, xpGainRetio_);

    // 実際に増えた経験値量を計算
    xp_gained_actually = xp_ - xp_pre;

    // 経験値が増えた分を減算
    xpGained_ -= xp_gained_actually;

    // レベルアップとレベルアップに必要な経験値量の計算
    while (xp_ >= xpMax_)
    {
        level_++;
        xp_ -= xpMax_;

        float gainXpMax = xpMax_ * 0.05f + level_ * 2;
        if (gainXpMax > 200.0f) gainXpMax = 200.0f;

        xpMax_ += gainXpMax;

        // イベント発行
        GameEventNotifier::GetInstance()->Notify("PlayerLevelUp", nullptr);
    }
}

void Player::UpdateOpacityByCameraDistance()
{
    // カメラとの距離に応じて透明度を調整する処理
    auto camera = *Object3dBasic::GetInstance()->GetCamera();
    Vector3 cameraPos = camera->GetTranslate();
    float distance = (transform_.translate - cameraPos).Length();
    // 透明度の計算（距離が近いほど透明、遠いほど不透明）
    float opacity = std::clamp((distance / distance_start_lerp_) - 1.0f, 0.0f, 1.0f);
    model_->SetMaterialColor({1.0f, 1.0f, 1.0f, opacity});
}

void Player::ChangeAimMode(bool isMouseAim)
{
    if (isMouseAim)
    {
        --countCursorVisible_;
        if (countCursorVisible_ == 0)
        {
            SetCursorPos(ORIGIN.x, ORIGIN.y);
            ShowCursor(false);
            mouseAim_ = true;
        }
    }
    else
    {
        ++countCursorVisible_;
        if (countCursorVisible_ == 1)
        {
            SetCursorPos(ORIGIN.x, ORIGIN.y);
            ShowCursor(true);
            mouseAim_ = false;
        }
    }
}

void Player::ChangeAimModeForce(bool _isMouseAim)
{
    if (_isMouseAim)
    {
        SetCursorPos(ORIGIN.x, ORIGIN.y);
        while (countCursorVisible_ > 0)
        {
            --countCursorVisible_;
            ShowCursor(false);
        }
        mouseAim_ = true;
    }
    else
    {
        SetCursorPos(ORIGIN.x, ORIGIN.y);
        while (countCursorVisible_ < 1)
        {
            ++countCursorVisible_;
            ShowCursor(true);
        }
        mouseAim_ = false;
    }
}

void Player::ToggleAimModeForce()
{
    if (mouseAim_)
    {
        ChangeAimModeForce(false);
    }
    else
    {
        ChangeAimModeForce(true);
    }
}

void Player::DrawDebug()
{
    weapon_->DrawDebug();
}