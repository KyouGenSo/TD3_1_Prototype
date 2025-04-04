#pragma once
#include "GameScene/Object/Bullets/BulletBase.h"

class MachineGunBullet : public BulletBase{
    struct Bullet{
    private:
        Vector3 origin {};

        std::unique_ptr<Object3d> model_;
        std::unique_ptr<Collision::Collider> collider_;

        Transform transform_ = {};
        Vector3 forward_ = {};
        float speed_ = 0.0f;

        bool dead = false;

    public:
        Bullet* Initialize();
        void Update();
        void Draw();

        Bullet* SetOriginalPosition(const Vector3& _pos);
        Bullet* SetRotate(const Vector3& _rotate);
        Bullet* SetForward(const Vector3& _forward);
        Bullet* SetSpeed(float _speed);
        bool IsDead() const {return dead;}
        void OnCollisionTrigger(const Collider* _other);
    };

    Vector3 origin{};
    std::vector<std::unique_ptr<Bullet>> bullets_ = {};
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void OnCollisionTrigger(const Collider* _collider) override;

protected:
    void InitializeNormal() override;
    void InitializeChain() override;
    void UpdateNormal() override;
    void UpdateChain() override;
};

