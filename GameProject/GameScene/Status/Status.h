#pragma once
#include <string>

class Status
{
public:
    Status() = default;
    ~Status() = default;

    void Initalize();
    void Update();
    void ImGui(const std::string& _name);

    // Getter
    int getHp() const                   { return hp_; }
    int getMaxHp() const                { return maxHp_; }
    int getAttack() const               { return attack_; }
    int getDefence() const              { return defence_; }
    int getSpeed() const                { return speed_; }
    int getLevel() const                { return level_; }
    int getExp() const                  { return exp_; }
    int getMaxExp() const               { return maxExp_; }

    // Setter
    Status& setHp(int hp)               { hp_ = hp; return *this; }
    Status& setMaxHp(int maxHp)         { maxHp_ = maxHp; return *this; }
    Status& setAttack(int attack)       { attack_ = attack; return *this; }
    Status& setDefence(int defence)     { defence_ = defence; return *this; }
    Status& setSpeed(int speed)         { speed_ = speed; return *this; }
    Status& setLevel(int level)         { level_ = level; return *this; }
    Status& setExp(int exp)             { exp_ = exp; return *this; }
    Status& setMaxExp(int maxExp)       { maxExp_ = maxExp; return *this; }

    // Adder
    void AddHp(int hp)                  { hp_ += hp; }
    void AddAttack(int attack)          { attack_ += attack; }
    void AddDefence(int defence)        { defence_ += defence; }
    void AddSpeed(int speed)            { speed_ += speed; }
    void AddLevel(int level)            { level_ += level; }
    void AddExp(int exp)                { exp_ += exp; }


public:
    void OnCollision(const Status& _status);

private:
    int hp_ = 0;
    int maxHp_ = 0;
    int attack_ = 0;
    int defence_ = 0;
    int speed_ = 0;
    int level_ = 0;
    int exp_ = 0;
    int maxExp_ = 0;
};