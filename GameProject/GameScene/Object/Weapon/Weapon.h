#pragma once
#include "GameScene/Object/Object.h"

#include <memory>
#include <list>

#include <Collision/Collider.h>
#include <GameScene/Object/Bullets/BulletBase.h>
#include <GameSystem/Chain/Chain.h>
#include <Vector3.h>
#include <Camera.h>
#include <Object3d.h>
#include <Collision/CollisionManager.h>

class WeaponBase : public Object
{
public:
    virtual void Fire();
    virtual void Fire(const Vector3& _position);

    void Initialize();
    void Update() override;
    void Draw() override = 0;
    void DrawDebug();


public:
    void SetChain(Chain* _chainManager) { pChain_ = _chainManager; }


protected:
    std::list<std::unique_ptr<BulletBase>> bullets_;
    Chain* pChain_;
    uint32_t sound_fire_ = 0;
    std::unique_ptr<Collision::Ray> pRayToReticle_ = nullptr;


protected:
    void AddNewBullet(std::unique_ptr<BulletBase> _bullet, const Vector3& _spawnPosition);
    void DeleteDeadBullet();
    void EnableSound() { isEnableSound_ = true; }


private:
    bool isEnableSound_ = false;
    bool isHitRayToReticle_ = false;
    Vector3 cameraForward_ = {};
    Vector3 forward_ = {};
    Vector3 cameraPosition_ = {};
    Collision::Manager::RayHitData hitdata_ = {};
    void UpdateRay();


private:
    Collision::Manager* pCollisionManager_ = nullptr;
};

