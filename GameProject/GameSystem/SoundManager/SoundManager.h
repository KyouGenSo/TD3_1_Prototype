#pragma once

#include <json.hpp>
#include <string>
#include <unordered_map>
#include <Audio.h>
#include <vector>
#include <Utility/RandomGenerator/RandomGenerator.h>
#include <Timer/Timer.h>

class SoundManager
{
public:
    struct SoundData
    {
        std::string path    = {};
        float       volume  = 0.0f;
        float       pitch   = 0.0f;
        bool        isLoop  = false;
        uint32_t    handle  = 0u;
    };


public:
    SoundManager(SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
    SoundManager(SoundManager&&) = delete;
    SoundManager& operator=(SoundManager&&) = delete;

    static SoundManager* GetInstance()
    {
        static SoundManager instance;
        return &instance;
    }
    void Initialize(const std::string& _filename);
    void Finalize();
    void LoadAll();
    void ImGui();

    SoundData SearchSoundData(const std::string& _name) const;
    uint32_t Play(const std::string& _name) const;
    uint32_t Play(const uint32_t _handle) const;
    uint32_t PlayNoLoop(const uint32_t _handle) const;


private:
    using json = nlohmann::json;
    const std::string kJsonDir = "Resources/Json/";

    json jsonData_;
    std::unordered_map<std::string, SoundData> soundDataMap_;

    // デバッグ用
    std::unordered_map<uint32_t, uint32_t> playHandleMap_;


private:
    SoundManager() = default;
    ~SoundManager() = default;
    void LoadFromFile(const std::string& _filename);
    void DecodeToSoundData();

private:
    Audio* pAudio_ = nullptr;
};

class SoundGroup
{
public:
    SoundGroup() = default;
    ~SoundGroup() = default;
    void Initialize();
    void Finalize() const;
    void Update();
    void AddSound(const uint32_t _handle);
    void Play() { enabled_ = true; }
    void Stop() { enabled_ = false; }


public:
    void SetInterval(float _interval) { interval_ = _interval; }


private:
    bool enabled_ = false;
    std::vector<uint32_t> soundGroup_;
    uint32_t currentPlayHandle_ = 0u;
    std::unique_ptr<Timer> pTimer_ = nullptr;
    float interval_ = 1.0f;

private:
    RandomGenerator* pRandomGenerator_ = nullptr;
};