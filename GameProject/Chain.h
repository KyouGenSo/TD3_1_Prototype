#pragma once
#include <memory>
#include <vector>

#include "Player.h"
#include "Weapon.h"

class Chain{
public:
	enum class Type{
        RocketLauncher,
        MachineGun,
		Lightning,
	};

private:
	std::vector<Type> types;
	std::unordered_map<Type, std::unique_ptr<Weapon>> weapons_;

	Player* pOwner_ = nullptr;
public:
	void Initialize();
	void Fire();

    // factory method
	void Add(Type type);
};

