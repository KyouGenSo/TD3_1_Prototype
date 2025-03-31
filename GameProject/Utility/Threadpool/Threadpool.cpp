#include "Threadpool.h"

#include <imgui.h>

void Threadpool::Initialize(uint32_t _numThreads)
{
    threads_.clear();
    for (uint32_t i = 0; i < _numThreads; ++i)
    {
        AddThread("Thread" + std::to_string(i));
    }
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

void Threadpool::ImGui()
{
    ImGui::Begin("Threadpool");

    ImGui::Text("Thread Count: %d", threads_.size());
    ImGui::Text("Task Count: %d", taskQueue_.size());

    for (auto& [name, thread] : threads_)
    {
        ImGui::Text(name.c_str());
        ImGui::Text("ID: %d", thread->get_id());
        ImGui::Text("Time: %.1f", timers_[thread->get_id()].GetNow<float>());
    }
    ImGui::End();
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
        timers_[std::this_thread::get_id()].Reset();
        timers_[std::this_thread::get_id()].Start();
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cv_.wait(lock, [this] { return !taskQueue_.empty() || stop_; });

            if (stop_ && taskQueue_.empty()) return;

            task = std::move(taskQueue_.front());
            taskQueue_.pop();
        }

        if (stop_) return;

        task();
    }
}


