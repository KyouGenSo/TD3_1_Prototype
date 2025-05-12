#pragma once
#include "Sprite.h"
#include "GameScene/Object/Object.h"

class Player;

class Crosshair final : public Object{
    std::unique_ptr<Sprite> sprite_;
    std::unique_ptr<Collision::Ray> ray_;

    Player* pPlayer_ = nullptr;

    const float REACH = 100.f;
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;

    void SetPlayer(Player* _player);

    Vector3 GetAimPosition() const {
        return transform_.translate;
    }

private:
    void UpdateRay();
    void NoHit();
};

