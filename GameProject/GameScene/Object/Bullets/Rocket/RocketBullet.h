#pragma once
#include <GameScene/Object/Bullets/BulletBase.h>

#include <Timer/Timer.h>

class RocketBullet : public BulletBase{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;

    void OnCollisionTrigger(const Collider* _other) override;

private:
    void InitializeNormal() override;
    void InitializeChain() override;
    void UpdateNormal() override;
    void UpdateChain() override;

    std::unique_ptr<Collider> explosion_;

    Vector4 color_ = {1,1,1,1};

    Timer timer_;
};

