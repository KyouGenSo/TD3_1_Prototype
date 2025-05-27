#pragma once
#include <GameScene/Object/Bullets/BulletBase.h>
#include "EmitterManager.h"

class RocketBullet : public BulletBase
{
    class Explosion : public Object {
        bool enable_ = false;

    public:
        void Init();
        void Disable();
        bool IsEnabled() const;
        void Draw() override;
    };

    class CB{
        std::unique_ptr<Explosion> explosion_;

        uint16_t remaining_ = 3;

        const float INTERVAL = 0.5f;

        float exInterval_ = INTERVAL;

        Vector3 position_ {};

        EmitterManager* emitter_ = nullptr;

    public:
        void Init();
        void Update();
        void SetPosition(const Vector3& pos);
        bool IsFinish() const;

        void SetEmitter(EmitterManager* _emitter);
    };

    std::unique_ptr<Explosion> exImpl_ = nullptr;

    std::unique_ptr<CB> cb_ = nullptr;
public:
    RocketBullet();
    void Initialize() override;
    void Update() override;
    void Draw() override;

    void OnCollisionTrigger(const Collision::Collider* _other) override;

private:
    void InitializeNormal() override;
    void InitializeChain() override;
    void UpdateNormal() override;
    void UpdateChain() override;
};

