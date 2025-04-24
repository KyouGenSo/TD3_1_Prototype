#pragma once
#include <string>
#include <json.hpp>

#include "../IReinforcement.h"

class ReinforcementManager
{
private:
    using json = nlohmann::json;

public: 
    const std::string kReinforcementDir_ = "Resources/Json/Reinforcement/";

    ReinforcementManager(ReinforcementManager&) = delete;
    ReinforcementManager& operator=(const ReinforcementManager&) = delete;
    ReinforcementManager(ReinforcementManager&&) = delete;
    ReinforcementManager& operator=(ReinforcementManager&&) = delete;

    static ReinforcementManager* GetInstance()
    {
        static ReinforcementManager instance;
        return &instance;
    }

    void Initialize(const std::string& _filename);
    void Notify(const std::string& _event);
    void RegisterReinforcement(IReinforcement* _reinforcement);
    void UnregisterReinforcement(IReinforcement* _reinforcement);

public: // Getter

    const json& GetReinforcementData(const std::string& _filename) const
    {
        return reinforcementData_.at(_filename);
    }


private:
    ReinforcementManager() = default;
    ~ReinforcementManager() = default;

    void LoadFromFile(const std::string& _filename);

private:
    std::unordered_map<std::string, json> reinforcementData_;
    std::vector<IReinforcement*> reinforcementList_;
};