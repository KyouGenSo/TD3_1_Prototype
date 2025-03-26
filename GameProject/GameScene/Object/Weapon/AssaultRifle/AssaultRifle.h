#pragma once
#include "GameScene/Object/Weapon/Weapon.h"

class AssaultRifle : public WeaponBase{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Fire() override;
};

