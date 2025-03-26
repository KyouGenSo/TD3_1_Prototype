#pragma once
#include "GameScene/Object/Object.h"

#include <memory>
#include <list>

#include <GameScene/Object/Bullets/BulletBase.h>
#include <GameSystem/Chain/Chain.h>

class WeaponBase : public Object
{
public:
    virtual void Fire();

    void Update() override = 0;
    void Draw() override = 0;


public:
    void SetChain(Chain* _chainManager) { pChain_ = _chainManager; }


protected:
    std::list<std::unique_ptr<BulletBase>> bullets_;
    Chain* pChain_;


protected:
    void Initialize() override = 0;
    void AddNewBullet(std::unique_ptr<BulletBase> _bullet);
    void DeleteDeadBullet();
};

