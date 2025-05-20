#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <list>
#include <any>

class GameEventNotifier
{
public:
    struct FunctionData
    {
        std::function<void(std::any)> func = {};
        uint32_t id = 0u;
    };

public:
    GameEventNotifier(const GameEventNotifier&) = delete;
    GameEventNotifier& operator=(const GameEventNotifier&) = delete;
    GameEventNotifier(GameEventNotifier&&) = delete;
    GameEventNotifier& operator=(GameEventNotifier&&) = delete;

    static GameEventNotifier* GetInstance()
    {
        static GameEventNotifier instance;
        return &instance;
    }

    uint32_t RegisterCallback(const std::string& _eventName, std::function<void(std::any)> _func);
    void UnregisterCallback(const std::string& _eventName, uint32_t _id);

    void Notify(const std::string& _eventName, std::any _data);


private:
    std::unordered_map<std::string, std::list<FunctionData>> m_callbacks;
    uint32_t m_idCounter = 0;

private:
    GameEventNotifier() = default;
    ~GameEventNotifier() = default;
};