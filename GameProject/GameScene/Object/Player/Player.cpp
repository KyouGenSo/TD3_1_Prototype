#include "Player.h"

#include "imgui.h"
#include "Input.h"

#include <ModelManager.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>

// DEBUG
#include <GameScene/Object/Weapon/RocketLauncher/RocketLauncher.h>
#include <QuatFunc.h>

void Player::Initialize() {
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

    collider_ = std::make_unique<Collider>(this);
    collider_->SetEvent([this](const Collider* pCol){this->OnCollision(pCol); });

    chainManager_ = std::make_unique<ChainManager>();
    chainManager_->Initialize();

    /// !!Debug!!
    chainManager_->SetChain(WeaponType::RocketLauncher, WeaponType::Assault, WeaponType::None, WeaponType::None);
    weapon_ = std::make_unique<RocketLauncher>();
    weapon_->SetChainManager(chainManager_.get());
}

void Player::Update() {

    deltaTime_ = DeltaTimeManager::GetInstance()->GetDeltaTime(0);

    UpdateInputCommands();
    UpdateMovement();
    
    weapon_->SetPosition(transform_.translate);
    weapon_->SetRotation(transform_.rotate);
    weapon_->Update();
    chainManager_->Update();

    /// Model Update
    model_->SetScale(transform_.scale);
    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
}

void Player::Draw() {
    weapon_->Draw();
    model_->Update();
    model_->Draw();
}

void Player::Finalize() {
}

void Player::ImGui() {

    chainManager_->ImGui();

    if (ImGui::Begin("Player"))
    {
        if (ImGui::TreeNode("Transform"))
        {
            ImGui::DragFloat3("Position", &transform_.translate.x, 0.1f);
            ImGui::DragFloat3("Rotation", &transform_.rotate.x, 0.1f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Common"))
        {
            ImGui::DragFloat("JumpPower", &jumpPower_, 0.01f);
            ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void Player::OnCollision(const Collider* pCollider) {
}

void Player::UpdateInputCommands()
{
    // Attack
    if (pInput_->PushKey(DIK_RETURN) || pInput_->PushButton(JOY_BUTTON1)){
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
    Vector3 move{};
    if(Input::GetInstance()->IsConnect()){
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
        move += forward_ * moveSpeed_ * static_cast<float>(directionForward) * deltaTime_;
        move += right * moveSpeed_ * static_cast<float>(directionRight) * deltaTime_;
    }


    // Jump
    if (isGround_){
        if(Input::GetInstance()->TriggerKey(DIK_SPACE)){
            acceleration_.y += jumpPower_;
            isGround_ = false;
        }
    }
    else
    {
        // 重力を加算
        acceleration_.y += GRAVITY;
    }

    // 速度を加算
    velocity_ += acceleration_;
    move_ += velocity_ * deltaTime_;

    // 座標を代入
    transform_.translate += move;

    if (transform_.translate.y < floor_)
    {
        transform_.translate.y = floor_;
        isGround_ = true;
        velocity_.y = floor_;
    }

    // 加速度初期化
    acceleration_ = {};
}

