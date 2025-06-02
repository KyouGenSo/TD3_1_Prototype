#pragma once
#include <string>
#include <json.hpp>

#include "../IReinforcement.h"
#include <utility/RandomGenerator/RandomGenerator.h>

struct ReinforcementData
{
    std::string name;
    std::string description;
    std::string imagepath;
};

class ReinforcementManager
{
private:
    using json = nlohmann::json;

public:
    const std::string kFilename_json_status_ = "StatusReinforcement.json";

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
    void Update();
    void Notify(const std::string& _event);
    void RegisterReinforcement(IReinforcement* _reinforcement);
    void UnregisterReinforcement(IReinforcement* _reinforcement);

    ReinforcementData GetRandomCard(const std::string& _filename) const;

public: // Getter

    const json& GetReinforcementData(const std::string& _filename) const
    {
        return reinforcementData_.at(_filename);
    }


private:
    ReinforcementManager() = default;
    ~ReinforcementManager() = default;

    void LoadFromFile(const std::string& _filename);
    void LoadImages(const std::string& _filename);

private:
    std::unordered_map<std::string, json> reinforcementData_;
    std::vector<IReinforcement*> reinforcementList_;
};