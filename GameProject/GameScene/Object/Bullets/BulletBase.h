#pragma once
#include <memory>
#include <mutex>

#include "GameScene/Object/Object.h"
#include <GameSystem/Chain/Chain.h>
#include <Timer/Timer.h>

#include "Collision/Collider.h"

class BulletBase : public Object
{
public:
    void Initialize() override;
    void Update() override;
    virtual void Fire();
    void OnCollisionTrigger(const Collision::Collider* _collider) override = 0;
    bool IsDead();


public: /// Setter
    void SetIsChainBullet(bool _flag) { isChainBullet_ = _flag; }
    void SetChainManager(Chain* _chainManager) { pChainManager_ = _chainManager; }
    void SetForward(const Vector3& _forward) { forward_ = _forward; }

private:
    std::atomic<bool> rdy_;
protected:
    std::mutex mutex_;
    std::unique_ptr<BulletBase> pNext_ = nullptr;
    std::unique_ptr<Timer> pLifeTimer_ = nullptr;
    std::unique_ptr<Timer> pNextBulletTimer_ = nullptr;
    Vector3 forward_ = {};
    bool isChainBullet_ = false;
    WeaponType type_ = WeaponType::None;
    float speed_ = 1.f;
    float lifeTime_ = 150.0f / speed_ * 0.0166f;

protected:
    virtual void InitializeNormal() = 0;
    virtual void InitializeChain() = 0;
    virtual void UpdateNormal() = 0;
    virtual void UpdateChain() = 0;

    void Next();

    // ライフタイムを算出
    void CalcLifeTime() { lifeTime_ = 150.0f / speed_ * 0.0166f; }
    // ライフタイムをチェック
    bool CheckLifeTime() const;

protected:
    Chain* pChainManager_ = nullptr;

private:
    void CreateNextBullet();
    void SetNextBullet(std::unique_ptr<BulletBase> _bullet);
    // クールタイムがあがっていればtrueを返す
    bool CheckCoolTime();
};
