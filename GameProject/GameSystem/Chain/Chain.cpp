#include "Chain.h"

#ifdef _DEBUG
#include <imgui.h>

#include <algorithm>
#endif // _DEBUG

WeaponType Chain::GetNextWeapon(WeaponType _weaponType) const {
    auto n = std::next(std::ranges::find(data_, _weaponType));
    if(n != data_.end()) {
        return *n;
    }

    return WeaponType::None;
}

bool Chain::IsLastWeapon(WeaponType _weaponType) const
{
    if (data_.back() == _weaponType) return true;
    if (GetNextWeapon(_weaponType) == WeaponType::None) return true;
    return false;
}

void Chain::Data(WeaponType _1, WeaponType _2, WeaponType _3, WeaponType _4)
{
    data_[0] = _1;
    data_[1] = _2;
    data_[2] = _3;
    data_[3] = _4;
}

void Chain::OnAttacked(WeaponType _weaponType)
{
    coolTimeCounter_[_weaponType].Reset();
    coolTimeCounter_[_weaponType].Start();
}

void Chain::ImGui()
{
#ifdef _DEBUG
    if (ImGui::Begin("Chain Debug"))
    {
        ImGui::SeparatorText("CoolTime");
        ImGui::Text("RocketLauncher : %.2f", coolTimes_[WeaponType::RocketLauncher]);
        ImGui::Text("Assault : %.2f", coolTimes_[WeaponType::Assault]);
        ImGui::Text("MachineGun : %.2f", coolTimes_[WeaponType::MachineGun]);
        ImGui::Text("Thunder : %.2f", coolTimes_[WeaponType::Thunder]);
    }
    ImGui::End();
#endif

}

void Chain::Initialize() 
{
    for (auto& WeaponType : CHAIN_ARRAY)
    {
        coolTimeCounter_[WeaponType].Reset();
        coolTimeCounter_[WeaponType].Start();
    }

    Data(WeaponType::Assault);
}

void Chain::Update()
{
    /// クールタイムの更新
    for (const auto& type : data_)
    {
        if (type == WeaponType::None) continue;
        auto now = coolTimeCounter_[type].GetNow<float>();
        float cooltime = static_cast<float>(COOLTIMES.at(type) - now);

        /// クールタイムがあがっていれば0にする
        cooltime = std::max<float>(cooltime, 0);

        coolTimes_[type] = cooltime;
    }
}
