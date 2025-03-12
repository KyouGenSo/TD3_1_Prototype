#include "ChainManager.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

bool ChainManager::IsLastWeapon(WeaponType _weaponType) const
{
    if (chain_.back() == _weaponType) return true;
    if (GetNextWeapon(_weaponType) == WeaponType::None) return true;
    return false;
}

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

void ChainManager::ImGui()
{
    if (ImGui::Begin("Chain Debug"))
    {
        ImGui::SeparatorText("CoolTime");
        ImGui::Text("RocketLauncher : %.2f", coolTimes_[WeaponType::RocketLauncher]);
        ImGui::Text("MachineGun : %.2f", coolTimes_[WeaponType::MachineGun]);
        ImGui::Text("Lightning : %.2f", coolTimes_[WeaponType::Lightning]);
    }
    ImGui::End();
}

void ChainManager::Initialize() 
{
    for (auto& WeaponType : CHAIN_ARRAY)
    {
        coolTimeCounter_[WeaponType].Reset();
        coolTimeCounter_[WeaponType].Start();
    }
}

void ChainManager::Update()
{
    /// クールタイムの更新
    for (const auto& type : chain_)
    {
        if (type == WeaponType::None) continue;
        auto now = coolTimeCounter_[type].GetNow();
        float cooltime = static_cast<float>(COOLTIMES.at(type) - now);

        /// クールタイムがあがっていれば0にする
        if (cooltime <= 0) cooltime = 0;

        coolTimes_[type] = cooltime;
    }
}
