#include "Threadpool.h"

void Threadpool::Initialize()
{
    threads_.clear();
}

void Threadpool::AddThread(const std::string& _name)
{
    threads_[_name] = std::make_unique<std::thread>(&Threadpool::Worker, this);
}

void Threadpool::AddTask(const std::function<void()>& _task)
{
    std::unique_lock<std::mutex> lock(mtx_);
    taskQueue_.push(_task);
    cv_.notify_one();
}

Threadpool::~Threadpool()
{
    {
        std::unique_lock<std::mutex> lock(mtx_);
        stop_ = true;
        cv_.notify_all();
    }  // ここでロックを解放

    // ロックを解放した状態でjoinを呼び出す
    for (auto& [name, thread] : threads_)
    {
        if (thread->joinable()) {
            thread->join();
        }
    }
}

void Threadpool::Worker()
{
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cv_.wait(lock, [this] { return !taskQueue_.empty() || stop_; });

            if (stop_ && taskQueue_.empty()) return;

            task = std::move(taskQueue_.front());
            taskQueue_.pop();
            task();
        }
    }
}


