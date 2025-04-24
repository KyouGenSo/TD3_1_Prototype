#pragma once

#include <GameScene/Object/Bullets/BulletBase.h>
#include <array>

#include "Collision/Collider.h"

class AssaultBullet : public BulletBase
{
private:
    class Bullet
    {
    public:
        void Initialize();
        void Update();
        void Draw();
        void OnCollisionTrigger(const Collision::Collider* _other);
        void SetPosition(const Vector3& _pos) { transform_.translate = _pos; }
        void SetRotate(const Vector3& _rotate) { transform_.rotate = _rotate; }
        void SetForward(const Vector3& _forward) { forward_ = _forward; }
        void SetSpeed(float _speed) { speed_ = _speed; }

        bool IsHit();
    private:
        std::unique_ptr<Timer> pLifeTimer_ = nullptr;
        std::unique_ptr<Object3d> model_ = nullptr;
        std::unique_ptr<Collision::Collider> collider_;
        Transform transform_ = {};
        Vector3 forward_ = {};
        float speed_ = 0.0f;

        bool hit = false;
    };

public:
    void Initialize() override;
    void Update() override;
    void Draw() override;

    void OnCollisionTrigger(const Collision::Collider* _other) override;

private:
    std::unique_ptr<Bullet> bullet_;
    std::array<std::unique_ptr<Bullet>, 16> bullets_ = {};

private:
    void InitializeNormal() override;
    void InitializeChain() override;
    void UpdateNormal() override;
    void UpdateChain() override;
};
