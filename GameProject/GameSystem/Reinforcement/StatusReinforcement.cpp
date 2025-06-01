#include "StatusReinforcement.h"

#include "Manager/ReinforcementManager.h"
#include <Utility/JSON/jsonutl.h>
#include <iostream>

void StatusReinforcement::Initialize(const std::string& _cardName)
{
    ParseFromJson(_cardName);
}

void StatusReinforcement::Update()
{
    if (event_ == "interval")
    {
        if (!timer_.GetIsStart())
        {
            timer_.Reset();
            timer_.Start();
        }

        if (timer_.GetNow<float>() > interval_)
        {
            if (operatorType_ == "add")
            {
                ApplicationByAddition();
            }
            else if (operatorType_ == "mul")
            {

            }
            timer_.Reset();
        }
    }
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
    utl::json::try_assign(target, "interval", interval_);
}

void StatusReinforcement::ApplicationByAddition()
{
    if (statusType == "heal")
    {
        status_->AddHp(value_);
    }
    else if (statusType == "health")
    {
        if (status_->getHp() + value_ > status_->getMaxHp())
        {
            status_->setMaxHp(status_->getHp() + value_);
        }
        status_->AddHp(value_);
    }
    else if (statusType == "attack")
    {
        status_->AddAttack(value_);
    }
    else if (statusType == "defense")
    {
        status_->AddDefence(value_);
    }
    else if (statusType == "speed")
    {
        status_->AddSpeed(value_);
    }
    else if (statusType == "jump")
    {
        behaviorData_->num_jump_max_ += static_cast<uint32_t>(value_);
    }
    else if (statusType == "bullet_size")
    {
        behaviorData_->size_bullet_ += static_cast<uint32_t>(value_);
    }

    status_->Update();
}

void StatusReinforcement::ApplicationByMultiplication()
{
    if (statusType == "health")
    {
        status_->setMaxHp(status_->getMaxHp() * value_);
    }
    else if (statusType == "attack")
    {
        status_->setAttack(status_->getAttack() * value_);
    }
    else if (statusType == "defence")
    {
        status_->setDefence(status_->getDefence() * value_);
    }
    else if (statusType == "speed")
    {
        status_->setSpeed(status_->getSpeed() * value_);
    }
}

void StatusReinforcement::OnHit()
{
    if (event_ != "onHit") return;

    if (operatorType_ == "add")
    {
        ApplicationByAddition();
    }
    else if (operatorType_ == "mul")
    {
        ApplicationByMultiplication();
    }
}
