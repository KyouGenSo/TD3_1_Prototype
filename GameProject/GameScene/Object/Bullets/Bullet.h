#pragma once
#include <memory>

#include "GameScene/Object/Object.h"
#include "GameScene/Object/Collision/Collider.h"
#include <GameScene/System/ChainManager.h>


class Bullet : public Object
{
public:
    virtual void Initialize() = 0;
    virtual void Fire();
    virtual void OnCollisionTrigger(const Object* _other) = 0;


public: /// Setter
    void SetIsChainBullet(bool _flag) { isChainBullet_ = _flag; }
    void SetChainManager(ChainManager* _chainManager) { pChainManager_ = _chainManager; }


protected:
    std::unique_ptr<Collider> pCollider_ = nullptr;
    std::unique_ptr<Bullet> pNext_ = nullptr;
    bool isChainBullet_ = false;
    WeaponType type_;
    float speed_ = 1.f;


protected:
    virtual void AttackNormal() = 0;
    virtual void AttackChain() = 0;
    void CreateNextBullet();

    // クールタイムがあがっていればtrueを返す
    bool CheckCoolTime();


protected:
    ChainManager* pChainManager_;

private:
    void SetNextBullet(std::unique_ptr<Bullet> _bullet);
};
