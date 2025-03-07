#include "Chain.h"

#include "RocketLauncher/RocketLauncher.h"

void Chain::Initialize() {
    weapons_[Type::RocketLauncher] = std::make_unique<RocketLauncher>();
    weapons_[Type::MachineGun] = std::make_unique<RocketLauncher>();
    weapons_[Type::Lightning] = std::make_unique<RocketLauncher>();
}

void Chain::Fire() {
    if (types.empty()) return;

    const auto type = types.front();
    weapons_[type]->Fire();
}

void Chain::Add(const Type type) {
    types.push_back(type);
}
