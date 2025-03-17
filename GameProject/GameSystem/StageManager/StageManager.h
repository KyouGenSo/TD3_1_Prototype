#pragma once
#include <string>
#include <unordered_map>
#include <Transform.h>

struct StageData
{
    Transform playerTransform;
    Transform castleTransform;
    Transform bossTransform;
};

class StageManager
{
public:
    StageManager(const StageManager&) = delete;
    StageManager& operator=(const StageManager&) = delete;
    StageManager(StageManager&&) = delete;
    StageManager& operator=(StageManager&&) = delete;

    static StageManager* GetInstance()
    {
        static StageManager instance;
        return &instance;
    }

public:
    StageManager() = default;
    ~StageManager() = default;

    void Initialize();

    void SetCurrent(const std::string& _stageName)
    {
        currentStage_ = _stageName;
    }

    const std::string& GetCurrent() const
    {
        return currentStage_;
    }

    const StageData& GetCurrentStageData() const
    {
        return stageMap_.at(currentStage_);
    }

private:
    std::string currentStage_;
    std::unordered_map<std::string, StageData> stageMap_;
};