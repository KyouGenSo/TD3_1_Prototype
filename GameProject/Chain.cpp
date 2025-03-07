#include "Chain.h"

void ChainManager::SetChain(WeaponType _1, WeaponType _2, WeaponType _3, WeaponType _4)
{
    chain_[0] = _1;
    chain_[1] = _2;
    chain_[2] = _3;
    chain_[3] = _4;
}

void ChainManager::OnAttacked(WeaponType _weaponType)
{
    coolTimeCounter_[_weaponType].Reset();
    coolTimeCounter_[_weaponType].Start();
}

void ChainManager::Initialize() 
{
    for (auto& WeaponType : chain_)
    {
        coolTimeCounter_[WeaponType].Reset();
        coolTimeCounter_[WeaponType].Start();
    }
}

void ChainManager::Update()
{
    /// クールタイムの更新
    for (const auto& WeaponType : chain_)
    {
        auto now = coolTimeCounter_[WeaponType].GetNow();
        float cooltime = COOLTIMES.at(WeaponType) - now;

        /// クールタイムがあがっていれば0にする
        if (cooltime <= 0) cooltime = 0;

        coolTimes_[WeaponType] = cooltime;
    }
}
