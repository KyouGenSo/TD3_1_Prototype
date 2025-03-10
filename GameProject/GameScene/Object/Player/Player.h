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

    const float FLOOR = 0.5f;
    const float GRAVITY = -.16f;
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize();
    void ImGui();
    void OnCollision(const Object* pObject) override;
    void AddObserver(IObserver* _observer) { observers_.push_back(_observer); }

private:
    void UpdateInputCommands();
    void UpdateMovement();
};

