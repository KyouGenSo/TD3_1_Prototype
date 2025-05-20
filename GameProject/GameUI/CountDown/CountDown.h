#pragma once

#include <Sprite.h>
#include <Timer/Timer.h>
#include <array>
#include <memory>
#include <Vector2.h>

class CountDown
{
public:
    void Initialize();
    void Finalize();
    void Update();
    void Draw2D();

    void Start();
    void Reset();
    bool IsStart() const { return !isInit_; }
    bool IsEnd() const { return isEnd_; }

    void OnResize(Vector2 size);

private:
    Timer timer_ = {};
    Timer startTimer_ = {};

    std::array<std::unique_ptr<Sprite>, 3> numbers_;
    std::unique_ptr<Sprite> start_ = nullptr;

    Vector2 client_standard_ = { 1600.0f, 900.0f };

    int currentNumber_ = 2;

    bool isDrawStart_ = false;
    bool isInit_ = false;
    bool isEnd_ = false;

    float opacity_ = 1.0f;

    double changeInterval_ = 1.0;
    double startDuration_ = 2.0;
};