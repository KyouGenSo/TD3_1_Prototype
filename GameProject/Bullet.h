#pragma once
#include <memory>

#include "Collider.h"
#include "Object.h"


class Bullet : public Object
{
public:
    void Fire();
    virtual void OnCollisionTrigger(const Object* _other) = 0;

public: /// Setter
    void SetIsChainBullet(bool _flag) { isChainBullet_ = _flag; }

protected:
    std::unique_ptr<Collider> collider_ = nullptr;
    std::unique_ptr<Bullet> next_ = nullptr;
    bool isChainBullet_ = false;

protected:
    virtual void AttackNormal() = 0;
    virtual void AttackChain() = 0;
    virtual void CreateNextBullet() = 0;
};

