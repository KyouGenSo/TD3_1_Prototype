#include "Weapon.h"

void Weapon::Fire() {
    isRoot_ = true;
    Initialize();
}

void Weapon::Execute() {
    isRoot_ = false;
    Initialize();
}

bool Weapon::IsDead() {
    return pOwner_->IsDead();
}
