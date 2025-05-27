#define NOMINMAX
#include "Player.h"

#include "imgui.h"
#include "Input.h"

#include <ModelManager.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>
#include <GameScene/Object/Weapon/WeaponFactory.h>

#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>

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
        .setSpeed(1)
        .setDefence(0)
        .setMaxHp(100);
    statusCurrent_ = statusInit_;

    /// !!Debug!!
    weapon_ = std::make_unique<AssaultRifle>();
    weapon_->Initialize();
    weapon_->SetChain(chain_);
    weapon_->SetEmitter(emitter_);
    gravity_ = 1.8f;

    // コールバック登録
    id_callback_enemydead_ = GameEventNotifier::GetInstance()->RegisterCallback("EnemyDeadForXP", [this](std::any _gainedXP) {
        xpGained_ += std::any_cast<float>(_gainedXP);
    });

    id_callback_playerlevelup_ = GameEventNotifier::GetInstance()->RegisterCallback("PlayerLevelUp", [this]([[maybe_unused]]std::any _unused) {
        for (auto& obs : observers_)
        {
            obs->OnNotify("toggle_lvup");
            DeltaTimeManager::GetInstance()->SetDeltaTime(1, 0.0f);
        }
    });

    id_callback_chainconfirm_ = GameEventNotifier::GetInstance()->RegisterCallback("ChainConfirm", [this]([[maybe_unused]]std::any _unused) {
        this->OnChainConfirm();
        DeltaTimeManager::GetInstance()->SetDeltaTime(1, 1.0f / 60.0f);
    });

}

void Player::Update()
{
    deltaTime_ = DeltaTimeManager::GetInstance()->GetDeltaTime(1);

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

    auto* rfmManager = ReinforcementManager::GetInstance();
    for (auto& reinforcement : reinforcementList_)
    {
        rfmManager->UnregisterReinforcement(reinforcement.get());
    }
    if (mouseAim_) ShowCursor(true);
}

void Player::ImGui()
{
    Object::ImGui([&]() {
        statusInit_.ImGui("PlayerInit");
        statusCurrent_.ImGui("PlayerCurrent");

        if (ImGui::TreeNode("Common"))
        {
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
}

void Player::OnCollisionTrigger(const Collision::Collider* pCollider)
{
    Object::StatusUpdateOnCollision(pCollider);
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
    if (pInput_->PushMouse(0) || pInput_->PushButton(JOY_BUTTON1))
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
        ShowCursor(!mouseAim_);
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

void Player::DrawDebug()
{
    weapon_->DrawDebug();
}