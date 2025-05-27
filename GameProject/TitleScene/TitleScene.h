#pragma once
#include "BaseScene.h"
#include"Sprite.h"
#include"Object3d.h"
#include "EmitterManager.h"

#include <memory>

class TitleScene : public BaseScene
{
public: // メンバ関数

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
    void DrawWithoutEffect() override;

    /// <summary>
    /// ImGuiの描画
    /// </summary>
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

    std::unique_ptr<EmitterManager> emitterManager_ = nullptr;
    BoxEmitterParams emitterParams_ = {};

    BloomParam bloomParam_ = {};

    Vector2 pressPos{ .x = 300, .y = 600 };
    Vector2 pressSize{ .x = 500, .y = 100 };
    Vector2 titlePos{ .x = 300, .y = 50 };

    float pressAlpha = 0.f;
    float pressAlphaSpeed = 0.05f;
    bool pressAlphaIncreasing = true;

    std::unique_ptr<Sprite> button_;
    std::unique_ptr<Sprite> press_;
    std::unique_ptr<Sprite> guide_;
    std::unique_ptr<Sprite> bgSp_;
    std::unique_ptr<Sprite> titleSprite_;

    std::unique_ptr<Sprite> whiteBarVer_;
    std::unique_ptr<Sprite> whiteBarVer2_;
    std::unique_ptr<Sprite> whiteBarHor_;
    std::unique_ptr<Sprite> whiteBarHor2_;
    Vector2 whiteBarVerPos{ .x= 80.f, .y= 0.f};
    Vector2 whiteBarVerPos2{ .x= 1520.f, .y= 0.f};
    Vector2 whiteBarHorPos{ .x= 0.f, .y= 80.f };
    Vector2 whiteBarHorPos2{ .x= 0.f, .y= 880.f };

    Vector2 whiteBarVerSize{ .x= 5.f, .y= 2000.f };
    Vector2 whiteBarHorSize{ .x= 2000.f, .y= 5.f};

    uint32_t bgmPlayHandle_ = 0;
};
