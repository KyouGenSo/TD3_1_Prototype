#include "StatusReinforcement.h"

#include "Manager/ReinforcementManager.h"

#include <iostream>

void StatusReinforcement::Initialize(const std::string& _cardName)
{
    ParseFromJson(_cardName);
}

void StatusReinforcement::Apply()
{
    if (event_ == "onGet")
    {
        if (operatorType_ == "add")
        {
            ApplicationByAddition();
        }
        if (operatorType_ == "mul")
        {

        }
    }
}

void StatusReinforcement::Remove()
{
}

void StatusReinforcement::IncrementLevel()
{
}

void StatusReinforcement::DecrementLevel()
{
}

void StatusReinforcement::ParseFromJson(const std::string& _cardName)
{
    std::string filepath = ReinforcementManager::GetInstance()->kReinforcementDir_ + "StatusReinforcement.json";
    auto& j = ReinforcementManager::GetInstance()->GetReinforcementData(filepath);
    if (j.is_null())
    {
        std::cerr << "Failed to get reinforcement data: " << _cardName << std::endl;
        assert(false && "Failed to get reinforcement data");
        return;
    }

    // JSONからデータを取得
    nlohmann::json target = {};
    for (auto& data : j)
    {
        if (data["name"].get<std::string>() == _cardName)
        {
            target = data;
            break;
        }
    }

    name_ = target["name"].get<std::string>();
    description_ = target["description"].get<std::string>();
    statusType = target["status-type"].get<std::string>();
    operatorType_ = target["operatorType"].get<std::string>();
    value_ = target["value"].get<float>();
    rarity_ = target["rarity"].get<int>();
    event_ = target["event"].get<std::string>();
}

void StatusReinforcement::ApplicationByAddition()
{
    if (statusType == "health")
    {
        status_->AddHp(static_cast<int>(value_));
    }
    else if (statusType == "attack")
    {
        status_->AddAttack(static_cast<int>(value_));
    }
    else if (statusType == "defence")
    {
        status_->AddDefence(static_cast<int>(value_));
    }
    else if (statusType == "speed")
    {
        status_->AddSpeed(static_cast<int>(value_));
    }
}

void StatusReinforcement::ApplicationByMultiplication()
{
    if (statusType == "health")
    {
        status_->setMaxHp(status_->getMaxHp() * static_cast<int>(value_));
    }
    else if (statusType == "attack")
    {
        status_->setAttack(status_->getAttack() * static_cast<int>(value_));
    }
    else if (statusType == "defence")
    {
        status_->setDefence(status_->getDefence() * static_cast<int>(value_));
    }
    else if (statusType == "speed")
    {
        status_->setSpeed(status_->getSpeed() * static_cast<int>(value_));
    }
}
