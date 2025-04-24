#pragma once
#include "GameScene/Object/Object.h"

#include "Camera.h"
#include <Collision/Collider.h>
#include <GameSystem/Chain/Chain.h>
#include <GameScene/Object/Weapon/Weapon.h>
#include <Interfaces/IObserver.h>
#include <GameSystem/Reinforcement/IReinforcement.h>

class Player : public Object
{
    std::unique_ptr<Collision::Collider> collider_;

    Chain* chain_ = nullptr;
    std::unique_ptr<WeaponBase> weapon_ = nullptr;

    std::list<IObserver*> observers_ = {};
    std::list<std::unique_ptr<IReinforcement>> reinforcementList_ = {};

    Vector3 move_ = {};

    bool isGround_ = true;

    const float HEIGHT_HALF = 0.5f;

    float floor_ = 0.5f;
    float jumpPower_ = 20.0f; // ジャンプ力
    float moveSpeed_ = 35.0f; // 移動速度
    float frictionCoefficient_ = 5.0f; // 摩擦係数 (friction coefficient)

public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize();
    void ImGui();
    void OnCollision(const Collision::Collider* pCollider) override;
    void OnCollisionTrigger(const Collision::Collider* pCollider) override;
    void AddObserver(IObserver* _observer) { observers_.push_back(_observer); }
    void SetFloor(float _floor) { floor_ = _floor; }
    void SetChain(Chain* _chain) { chain_ = _chain; weapon_->SetChain(_chain); }
    void AddReinforcement(const std::string& _cardName);
    void OnChainConfirm();

private:
    void UpdateInputCommands();
    void UpdateMovement();
};

