#include "Player.h"

#include "imgui.h"
#include "Input.h"

#include <ModelManager.h>

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

    transform_ = {
        .scale = { 1.0f, 1.0f, 1.0f },
        .rotate = { 0.0f, 0.0f, 0.0f },
        .translate = { 0.0f, 0.5f, -180.0f },
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

    ImGui::Begin("Player");
    ImGui::Text("Position : ");
    ImGui::SameLine();
	ImGui::DragFloat3("##pos", &transform_.translate.x, 0.1f);
    ImGui::Text("Rotate   : ");
    ImGui::SameLine();
	ImGui::DragFloat3("##rotate", &transform_.rotate.x, 0.1f);
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
    // Movement
    constexpr float speed = 0.3f;
    Vector3 move{};
    if(Input::GetInstance()->IsConnect()){
        // Joycon Movement
    }
    else
    {
        Quaternion yaw = Quat::MakeRotateAxisAngle({ 0.0f, 1.0f, 0.0f }, transform_.rotate.y);
        Quaternion pitch = Quat::MakeRotateAxisAngle({ 1.0f, 0.0f, 0.0f }, transform_.rotate.x);

        Quaternion rotate = yaw * pitch;

        Vector3 forward_ = Quat::RotateVec3({ 0.0f, 0.0f, 1.0f }, rotate);
        Vector3 right = Quat::RotateVec3({ 1.0f, 0.0f, 0.0f }, rotate);

        move += forward_ * speed * (Input::GetInstance()->PushKey(DIK_W) - Input::GetInstance()->PushKey(DIK_S));
        move += right * speed * (Input::GetInstance()->PushKey(DIK_D) - Input::GetInstance()->PushKey(DIK_A));
    }

    // Jump
    if (isGround_){
        if(Input::GetInstance()->TriggerKey(DIK_SPACE)){
            move_.y = 1.6f;
            isGround_ = false;
        }
    }
    else
    {
        move_.y += GRAVITY;
        if (transform_.translate.y + move_.y <= FLOOR)
        {
            transform_.translate.y = FLOOR;
            move_.y = 0.0f;
            isGround_ = true;
        }
    }

    // 座標を代入
    transform_.translate += move;
}

