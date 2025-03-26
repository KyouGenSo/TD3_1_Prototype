#pragma once
#include <GameScene/Object/Weapon/Weapon.h>

class RocketLauncher final : public WeaponBase{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
    void Fire() override;
};

