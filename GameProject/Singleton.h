#pragma once
#include <cassert>
#include <mutex>

#include "SingletonFinalizer.h"

template <typename T>
class Singleton final{
    static T* instance_;
    static std::once_flag flag_;

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T* GetInstance();

private:
	Singleton() = default;
	~Singleton() = default;

	static void Create();
    static void Destroy();
};

template <typename T> T* Singleton<T>::instance_ = nullptr;
template <typename T> std::once_flag Singleton<T>::flag_;

template <typename T>
T* Singleton<T>::GetInstance() {
	std::call_once(flag_, Create);
	assert(instance_);
	return instance_;
}

template <typename T>
void Singleton<T>::Create() {
	instance_ = new T;
	SingletonFinalizer::AddFinalizer(&Destroy);
}

template <typename T>
void Singleton<T>::Destroy() {
	delete instance_;
	instance_ = nullptr;
}

