#ifndef ThunderBullet_HPP_
#define ThunderBullet_HPP_
#include "GameScene/Object/Bullets/BulletBase.h"

class ThunderBullet final : public BulletBase{
    class Bullet : public BulletBase{
        bool isExploded_ = false;

        Object* target_ = nullptr;
    public:
        void Draw() override;
        void Initialize() override;
        void Update() override;
        void OnCollisionTrigger(const Collision::Collider* _collider) override;
        void SetSpeed(float _speed);

    protected:
        void InitializeNormal() override;
        void InitializeChain() override;
        void UpdateNormal() override;
        void UpdateChain() override;
    };
    std::unique_ptr<Bullet> bullet_;
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void OnCollisionTrigger(const Collision::Collider* _collider) override;
    bool IsDeadAll() override;
protected:
    void InitializeNormal() override;
    void InitializeChain() override;
    void UpdateNormal() override;
    void UpdateChain() override;
}; // class ThunderBullet


#endif // ThunderBullet_HPP_
