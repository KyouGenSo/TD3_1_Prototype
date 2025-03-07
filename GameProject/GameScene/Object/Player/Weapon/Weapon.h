#pragma once
#include "GameScene/Object/Object.h"

#include <memory>
#include <list>

#include <Bullet.h>
#include <Chain.h>

class Weapon : public Object
{
public:
    virtual void Fire() = 0;

    void Update() override = 0;
    void Draw() override = 0;


public:
    void SetChainManager(ChainManager* _chainManager) { pChainManager_ = _chainManager; }


protected:
    std::list<std::unique_ptr<Bullet>> bullets_;
    ChainManager* pChainManager_ = nullptr;


protected:
    void Initialize() override = 0;
    void AddNewBullet(std::unique_ptr<Bullet> _bullet);
};

