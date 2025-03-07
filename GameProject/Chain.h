#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

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

public:
	void Initialize();
	void Fire();

    // factory method
	void Add(Type type);
};

