#include "CountDown.h"

#include <Math/NiVec2.h>
#include <WinApp.h>
#include <TextureManager.h>

template<typename T>
constexpr auto TO_VECTOR2(T _class) { return Vector2(_class.x, _class.y) ; }

void CountDown::Initialize()
{
    TextureManager::GetInstance()->LoadTexture("number_1.png");
    TextureManager::GetInstance()->LoadTexture("number_2.png");
    TextureManager::GetInstance()->LoadTexture("number_3.png");
    TextureManager::GetInstance()->LoadTexture("start.png");

    /// 数字 1 - 3 (256 x 256)
    for (int i = 0; i < 3; i++)
    {
        numbers_[i] = std::make_unique<Sprite>();
    }

    NiVec2 clientSize = { static_cast<float>(WinApp::clientWidth), static_cast<float>(WinApp::clientHeight) };
    NiVec2 harfpos = clientSize / 2.0f;

    numbers_[0]->Initialize("number_1.png");
    numbers_[0]->SetAnchorPoint({0.5f, 0.5f});
    numbers_[0]->SetPos(TO_VECTOR2(harfpos));
    
    numbers_[1]->Initialize("number_2.png");
    numbers_[1]->SetAnchorPoint({ 0.5f, 0.5f });
    numbers_[1]->SetPos(TO_VECTOR2(harfpos));
    
    numbers_[2]->Initialize("number_3.png");
    numbers_[2]->SetAnchorPoint({ 0.5f, 0.5f });
    numbers_[2]->SetPos(TO_VECTOR2(harfpos));

    start_ = std::make_unique<Sprite>();
    start_->Initialize("start.png");
    start_->SetAnchorPoint({ 0.5f, 0.5f });
    start_->SetPos(TO_VECTOR2(harfpos));

    timer_.Reset();
    startTimer_.Reset();

    isInit_ = true;
}

void CountDown::Finalize()
{
}

void CountDown::Update()
{
    if (isEnd_ || isInit_) return;

    if (timer_.GetNow<double>() > changeInterval_ && !isDrawStart_)
    {
        currentNumber_--;
        timer_.Reset();
        timer_.Start();
    }

    if (currentNumber_ < 0)
    {
        isDrawStart_ = true;
        currentNumber_ = 2;
        startTimer_.Reset();
        startTimer_.Start();
    }

    if (startTimer_.GetNow<double>() > startDuration_ && startTimer_.GetIsStart())
    {
        isEnd_ = true;
        isInit_ = true;
    }

    /// 透明度を変更
    if (!isDrawStart_)
    {
        float t = static_cast<float>(timer_.GetNow<double>() / changeInterval_);
        if (t > 1.0f) t = 1.0f;
        opacity_ = (1.0f - t) * 1.0f;
        numbers_[currentNumber_]->SetColor({ 1.0f, 1.0f, 1.0f, opacity_ });
        numbers_[currentNumber_]->Update();
    }
    else
    {
        if (startTimer_.GetNow<double>() > startDuration_ - 1.0)
        {
            float t = static_cast<float>(startTimer_.GetNow<double>() - (startDuration_ - 1.0) / 1.0);
            if (t > 1.0f) t = 1.0f;
            opacity_ = (1.0f - t) * 1.0f;
        }
        else
        {
            opacity_ = 1.0f;
        }
        start_->SetColor({ 1.0f, 1.0f, 1.0f, opacity_ });
        start_->Update();
    }
}

void CountDown::Draw2D()
{
    if (isEnd_|| isInit_) return;

    if (!isDrawStart_)      numbers_[currentNumber_]->Draw();
    else                    start_->Draw();
}

void CountDown::Start()
{
    timer_.Reset();
    timer_.Start();
    startTimer_.Reset();

    isDrawStart_ = false;
    isEnd_ = false;
    isInit_ = false;
    currentNumber_ = 2;
}

void CountDown::Reset()
{
    timer_.Reset();
    startTimer_.Reset();

    isInit_ = true;
}

void CountDown::OnResize(Vector2 _size)
{
    Vector2 harfpos = { _size.x / 2.0f, _size.y / 2.0f };
    Vector2 harfSize = {};

    Vector2 retio = { _size.x / client_standard_.x, _size.y / client_standard_.y };

    for (auto& number : numbers_)
    {
        number->SetPos(TO_VECTOR2(harfpos));
        auto size = number->GetSize();
        number->SetSize({ size.x * retio.x, size.y * retio.y });
    }

    start_->SetPos(TO_VECTOR2(harfpos));
    auto size = start_->GetSize();
    start_->SetSize({ size.x * retio.x, size.y * retio.y });
}
