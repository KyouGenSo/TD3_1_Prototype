#pragma once
#include "GameScene/Object/Object.h"

#include <memory>
#include <list>

#include <GameScene/Object/Bullets/BulletBase.h>
#include <GameScene/System/ChainManager.h>

class Weapon : public Object
{
public:
    virtual void Fire();

    void Update() override = 0;
    void Draw() override = 0;


public:
    void SetChainManager(ChainManager* _chainManager) { pChainManager_ = _chainManager; }


protected:
    std::list<std::unique_ptr<BulletBase>> bullets_;
    ChainManager* pChainManager_ = nullptr;


protected:
    void Initialize() override = 0;
    void AddNewBullet(std::unique_ptr<BulletBase> _bullet);
    void DeleteDeadBullet();
};

