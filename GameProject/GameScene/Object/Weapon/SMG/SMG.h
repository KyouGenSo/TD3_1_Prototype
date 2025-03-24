#pragma once
#include "GameScene/Object/Weapon/Weapon.h"

class SMG : public Weapon{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
};

