#pragma once

#include <GameScene/Object/Bullets/BulletBase.h>
#include <array>

class AssaultBullet : public BulletBase
{
private:
    struct Bullet
    {
    public:
        void Initialize();
        void Update();
        void Draw();
        void OnCollisionTrigger(const Collider* _other);
        void SetPosition(const Vector3& _pos) { transform_.translate = _pos; }
        void SetForward(const Vector3& _forward) { forward_ = _forward; }
        void SetSpeed(float _speed) { speed_ = _speed; }

    private:
        std::unique_ptr<Timer> pLifeTimer_ = nullptr;
        std::unique_ptr<Object3d> model_ = nullptr;
        Transform transform_ = {};
        Vector3 forward_ = {};
        float speed_ = 0.0f;
    };

public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Fire() override;

    void OnCollisionTrigger(const Collider* _other) override;

private:
    std::unique_ptr<Bullet> bullet_;
    std::array<std::unique_ptr<Bullet>, 16> bullets_ = {};

private:
    void InitializeNormal() override;
    void InitializeChain() override;
    void UpdateNormal() override;
    void UpdateChain() override;
};