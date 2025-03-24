#include "ChainViewModel.h"

void ChainViewModel::Initialize()
{
    chain_ = std::make_unique<Chain>();
    chain_->Initialize();
}

void ChainViewModel::UpdateChainData(const std::array<std::string, 4>& _chainData)
{
    WeaponType weaponType[4] = { WeaponType::None, WeaponType::None, WeaponType::None, WeaponType::None };
    
    for (size_t i = 0; i < _chainData.size(); ++i)
    {
        if (_chainData[i] == "RocketLauncher")
        {
            weaponType[i] = WeaponType::RocketLauncher;
        }
        else if (_chainData[i] == "Assault")
        {
            weaponType[i] = WeaponType::Assault;
        }
        else if (_chainData[i] == "MachineGun")
        {
            weaponType[i] = WeaponType::MachineGun;
        }
        else if (_chainData[i] == "Lightning")
        {
            weaponType[i] = WeaponType::Lightning;
        }
    }

    chain_->Data(weaponType[0], weaponType[1], weaponType[2], weaponType[3]);

    return;
}
