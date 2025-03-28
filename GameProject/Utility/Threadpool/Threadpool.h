#pragma once

#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <mutex>
#include <queue>

class Threadpool
{
public:
    Threadpool(Threadpool const&) = delete;
    Threadpool& operator=(Threadpool const&) = delete;
    Threadpool(Threadpool&&) = delete;
    Threadpool& operator=(Threadpool&&) = delete;

    static Threadpool* GetInstance()
    {
        static Threadpool instance;
        return &instance;
    }

    void Initialize();
    void AddThread(const std::string& _name);
    void AddTask(const std::function<void()>& _task);

private:
    Threadpool() = default;
    ~Threadpool();

private:
    using ThreadPtr = std::unique_ptr<std::thread>;

    std::mutex mtx_;
    std::unordered_map<std::string, ThreadPtr> threads_;
    std::condition_variable cv_;
    std::queue<std::function<void()>> taskQueue_;
    bool stop_ = false;

private:
    void Worker();
};