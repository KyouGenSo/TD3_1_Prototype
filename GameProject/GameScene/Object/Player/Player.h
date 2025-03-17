#pragma once
#include "GameScene/Object/Object.h"

#include "Camera.h"
#include <GameScene/System/ChainManager.h>
#include <GameScene/Object/Collision/Collider.h>
#include <GameScene/Object/Weapon/Weapon.h>
#include <Interfaces/IObserver.h>

class Player : public Object {
    std::unique_ptr<Collider> collider_;

    std::unique_ptr<ChainManager> chainManager_;
    std::unique_ptr<Weapon> weapon_;

    std::list<IObserver*> observers_;

    Vector3 move_{};

    bool isGround_ = true;

    const float HEIGHT_HALF = 0.5f;

    float floor_ = 0.5f;
    float jumpPower_ = 20.0f; // ジャンプ力
    float moveSpeed_ = 20.0f; // 移動速度
    float frictionCoefficient_ = 6.5f; // 摩擦係数

public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize();
    void ImGui();
    void OnCollision(const Collider* pCollider) override;
    void AddObserver(IObserver* _observer) { observers_.push_back(_observer); }
    void SetFloor(float _floor) { floor_ = _floor; }

private:
    void UpdateInputCommands();
    void UpdateMovement();
};

