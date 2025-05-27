#ifndef ThunderRod_HPP_
#define ThunderRod_HPP_
#include "GameScene/Object/Weapon/Weapon.h"


class ThunderRod final : public WeaponBase{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
}; // class ThunderRod

#endif // ThunderRod_HPP_
