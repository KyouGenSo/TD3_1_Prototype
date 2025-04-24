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
    void LoadFromFile(const std::string& _filename);

    // Getter
    float getHp() const                   { return hp_; }
    float getMaxHp() const                { return maxHp_; }
    float getAttack() const               { return attack_; }
    float getDefence() const              { return defence_; }
    float getSpeed() const                { return speed_; }
    float getLevel() const                { return level_; }
    float getExp() const                  { return exp_; }
    float getMaxExp() const               { return maxExp_; }

    // Setter
    Status& setHp(float hp)               { hp_ = hp; return *this; }
    Status& setMaxHp(float maxHp)         { maxHp_ = maxHp; return *this; }
    Status& setAttack(float attack)       { attack_ = attack; return *this; }
    Status& setDefence(float defence)     { defence_ = defence; return *this; }
    Status& setSpeed(float speed)         { speed_ = speed; return *this; }
    Status& setLevel(float level)         { level_ = level; return *this; }
    Status& setExp(float exp)             { exp_ = exp; return *this; }
    Status& setMaxExp(float maxExp)       { maxExp_ = maxExp; return *this; }

    // Adder
    void AddHp(float hp)                  { hp_ += hp; }
    void AddAttack(float attack)          { attack_ += attack; }
    void AddDefence(float defence)        { defence_ += defence; }
    void AddSpeed(float speed)            { speed_ += speed; }
    void AddLevel(float level)            { level_ += level; }
    void AddExp(float exp)                { exp_ += exp; }


public:
    void OnCollision(const Status& _status);

private:
    float hp_ = 0;
    float maxHp_ = 0;
    float attack_ = 0;
    float defence_ = 0;
    float speed_ = 0;
    float level_ = 0;
    float exp_ = 0;
    float maxExp_ = 0;
};