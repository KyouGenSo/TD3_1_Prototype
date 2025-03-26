#include "ChainManager.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

WeaponType ChainManager::GetNextWeapon(WeaponType _weaponType) const {
    auto n = std::next(std::ranges::find(chain_, _weaponType));
    if(n != chain_.end()) {
        return *n;
    }

    return WeaponType::None;
}

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
#ifdef _DEBUG
    if (ImGui::Begin("Chain Debug"))
    {
        ImGui::SeparatorText("CoolTime");
        ImGui::Text("RocketLauncher : %.2f", coolTimes_[WeaponType::RocketLauncher]);
        ImGui::Text("Assault : %.2f", coolTimes_[WeaponType::Assault]);
        ImGui::Text("MachineGun : %.2f", coolTimes_[WeaponType::MachineGun]);
        ImGui::Text("Lightning : %.2f", coolTimes_[WeaponType::Lightning]);
    }
    ImGui::End();
#endif

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
        auto now = coolTimeCounter_[type].GetNow<float>();
        float cooltime = static_cast<float>(COOLTIMES.at(type) - now);

        /// クールタイムがあがっていれば0にする
        if (cooltime <= 0) cooltime = 0;

        coolTimes_[type] = cooltime;
    }
}
