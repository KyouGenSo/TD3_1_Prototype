#include "Player.h"

#include "imgui.h"
#include "Input.h"

// DEBUG
#include <GameScene/Object/Weapon/RocketLauncher/RocketLauncher.h>

void Player::Initialize() {
	model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetCamera(pCamera_);
    model_->SetModel("AnimatedCube.gltf");

    transform_ = {
        {1,1,1},
        {0,0.f,0},
        {0,0,0}
    };

    collider_ = std::make_unique<Collider>(this);
    collider_->SetEvent([this](const Object* obj){this->OnCollision(obj); });

    chainManager_ = std::make_unique<ChainManager>();

    // !DEBUG
    chainManager_->SetChain(WeaponType::RocketLauncher, WeaponType::MachineGun, WeaponType::Lightning, WeaponType::None);
    weapon_ = std::make_unique<RocketLauncher>();
    weapon_->SetChainManager(chainManager_.get());
}

void Player::Update() {

    UpdateInputCommands();
    UpdateMovement();
    
    weapon_->Update();

    /// Model Update
    model_->SetScale(transform_.scale);
    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void Player::Draw() {
    model_->Draw();
}

void Player::Finalize() {
}

void Player::ImGui() {
    ImGui::Begin("Player");
    ImGui::Text("Position : ");
    ImGui::SameLine();
	ImGui::DragFloat3("##pos", &transform_.translate.x, 0.1f);
    ImGui::Text("Rotate   : ");
    ImGui::SameLine();
	ImGui::DragFloat3("##rotate", &transform_.rotate.x, 0.1f);
    ImGui::End();
}

void Player::OnCollision(const Object* pObject) {
}

void Player::UpdateInputCommands()
{
    // Attack
    if (Input::GetInstance()->PushKey(DIK_SPACE) || Input::GetInstance()->PushKey(JOY_BUTTON1)){
        weapon_->Fire();
    }
}

void Player::UpdateMovement()
{
    // Movement
    constexpr float speed = 0.3f;
    Vector3 move{};
    if(Input::GetInstance()->IsConnect()){
        // Joycon Movement
    } else{
        move += {
            static_cast<float>(Input::GetInstance()->PushKey(DIK_D) - Input::GetInstance()->PushKey(DIK_A)),
                0.f,
                static_cast<float>(Input::GetInstance()->PushKey(DIK_W) - Input::GetInstance()->PushKey(DIK_S))
        };
    }
    move_ = move.Normalize() * speed;

    // Jump
    if (isGround_){
        if(Input::GetInstance()->TriggerKey(DIK_SPACE)){
            move_.y = 1.6f;
            isGround_ = false;
        }
    }else{
        move_.y += GRAVITY;
        if (transform_.translate.y + move_.y <= FLOOR){
            transform_.translate.y = FLOOR;
            move_.y = 0;
            isGround_ = true;
        }
    }

    // À•W‚ð‘ã“ü
    transform_.translate += Mat4x4::TransFormNormal(pCamera_->GetWorldMatrix(), move_);
}

