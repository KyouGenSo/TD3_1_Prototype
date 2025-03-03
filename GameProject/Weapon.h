#pragma once

class Weapon{
protected:
	struct Status{
		
	};
	bool isRoot_ = false;
	Status status_;

public:
	void Fire();
	void Execute();

	virtual void Update() = 0;
    virtual void Draw() = 0;
protected:
	virtual void Initialize() = 0;
};

