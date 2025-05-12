#include "Crosshair.h"

#include "Collision/CollisionManager.h"
#include "GameScene/Object/Player/Player.h"
#include "Type/Singleton.h"

void Crosshair::Initialize() {
    Object::Initialize();

    sprite_ = std::make_unique<Sprite>();
    sprite_->Initialize("circle.png");

    sprite_->SetAnchorPoint({.x= 0.5f, .y= 0.5f});
    sprite_->SetPos({.x = static_cast<float>(WinApp::clientWidth) / 2.f, .y = static_cast<float>(WinApp::clientHeight) / 2.f});

    sprite_->SetSize({.x= 32.f, .y= 32.f});
}

void Crosshair::Update() {
    Object::Update();

    UpdateRay();

    sprite_->Update();
}

void Crosshair::Draw() {
    sprite_->Draw();
}

void Crosshair::SetPlayer(Player* _player) {
    pPlayer_ = _player;

    ray_ = std::make_unique<Collision::Ray>(Adaptor(pPlayer_->GetTransform().translate), Adaptor(pPlayer_->GetTransform().rotate), REACH);
}

void Crosshair::UpdateRay() {
    // Update
    if (pPlayer_ == nullptr)return;
    ray_->SetOrigin(Adaptor(pPlayer_->GetTransform().translate));
    ray_->SetDirection(Adaptor(pPlayer_->GetTransform().rotate));

    // Detection
    auto data = Singleton<Collision::Manager>::GetInstance()->RayCast(ray_.get()); //
    if (data.pair.first.empty() || data.pair.second.empty()) // No hit
    if ((Adaptor(data.hitPoint) - pPlayer_->GetTransform().translate).Length() <= 1.f)return; // Prevents penetration into buildings

    transform_.translate = Adaptor(data.hitPoint);
}

void Crosshair::NoHit() {
    transform_.translate = pPlayer_->GetTransform().translate + (pPlayer_->GetTransform().rotate * REACH);
    transform_.rotate = pPlayer_->GetTransform().rotate;
}
