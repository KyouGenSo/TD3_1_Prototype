#pragma once
#include "Weapon.h"
class RocketLauncher final : public Weapon{
public:
	void Update() override;
	void Draw() override;

protected:
	void Initialize() override;
};

