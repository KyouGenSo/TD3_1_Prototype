#pragma once

#include <BaseScene.h>
#include <memory>

#include "Camera.h"
#include "EmitterManager.h"
#include "EmitterStruct.h"
#include "Sprite.h"

class ResultScene : public BaseScene
{
public:
    ResultScene() = default;
    ~ResultScene() override = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void DrawWithoutEffect() override;
    void DrawImGui() override;

public: // 構造体
    struct BloomParam
    {
        float intensity;
        float threshold;
        float sigma;
        float sigmaIncreaSpeed;
        bool sigmaIncreasing;
        int kernelSize;
    };

private: // メンバ変数

    bool isDebug_ = false;

    Camera* camera_ = nullptr;

    EmitterManager* emitterManager_ = nullptr;
    BoxEmitterParams emitterParams_ = {};

    BloomParam bloomParam_ = {};

    Vector2 pressPos{ .x = 300, .y = 600 };
    Vector2 pressSize{ .x = 500, .y = 100 };
    Vector2 overPos{ .x = 300, .y = 50 };

    float pressAlpha = 0.f;
    float pressAlphaSpeed = 0.05f;
    bool pressAlphaIncreasing = true;

    std::unique_ptr<Sprite> press_ = nullptr;
    std::unique_ptr<Sprite> overText_ = nullptr;
    std::unique_ptr<Sprite> bgSp_ = nullptr;

    std::unique_ptr<Sprite> whiteBarVer_ = nullptr;
    std::unique_ptr<Sprite> whiteBarVer2_ = nullptr;
    std::unique_ptr<Sprite> whiteBarHor_ = nullptr;
    std::unique_ptr<Sprite> whiteBarHor2_ = nullptr;
    Vector2 whiteBarVerPos{ .x = 80.f, .y = 0.f };
    Vector2 whiteBarVerPos2{ .x = 1520.f, .y = 0.f };
    Vector2 whiteBarHorPos{ .x = 0.f, .y = 80.f };
    Vector2 whiteBarHorPos2{ .x = 0.f, .y = 880.f };

    Vector2 whiteBarVerSize{ .x = 5.f, .y = 2000.f };
    Vector2 whiteBarHorSize{ .x = 2000.f, .y = 5.f };
};
