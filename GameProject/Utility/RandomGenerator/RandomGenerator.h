#pragma once
#include <Vector3.h>
#include <random>


class RandomGenerator
{
public:
    template <class T, typename U>
    struct Range
    {
        U* min;
        U* max;
        size_t size = sizeof(T) / sizeof(U);
    };

public:
    RandomGenerator(RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;
    RandomGenerator(RandomGenerator&&) = delete;
    RandomGenerator& operator=(const RandomGenerator&&) = delete;

    static RandomGenerator* GetInstance()
    {
        static RandomGenerator instance;
        return &instance;
    }

    void Initialize();


public:
    /// <summary>
    /// Generate random value in range
    /// </summary>
    /// <typeparam name="T">Type of the result</typeparam>
    /// <typeparam name="U">Type of the range</typeparam>
    /// <param name="_range">Range of the random value</param>
    /// <returns>Random value</returns>
    template <class T, typename U>
    T Generate(Range<T, U> _range);
    
    template <typename T>
    T Generate(T _min, T _max);


private:
    std::random_device rd_;
    std::mt19937 gen_;


private:
    RandomGenerator() = default;
    ~RandomGenerator() = default;
};

template<class T, typename U>
inline T RandomGenerator::Generate(Range<T, U> _range)
{
    T result = {};
    U* pResult = reinterpret_cast<U*>(&result);

    for (size_t i = 0; i < _range.size; ++i) {
        std::uniform_real_distribution<U> dist(_range.min[i], _range.max[i]);
        pResult[i] = dist(gen_);
    }

    return result;
}

template<typename T>
inline T RandomGenerator::Generate(T _min, T _max)
{
    if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value)
    {
        std::uniform_real_distribution<T> dist(_min, _max);
        return dist(gen_);
    }
    else if constexpr (std::is_same<T, int>::value || std::is_same<T, unsigned int>::value)
    {
        std::uniform_int_distribution<T> dist(static_cast<int>(_min), static_cast<int>(_max));
        return dist(gen_);
    }
    else
    {
        return T();
    }
}
