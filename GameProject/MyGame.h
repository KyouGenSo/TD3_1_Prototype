#pragma once
#include"TakoFramework.h"
#include"Vector2.h"
#include <NiUI/Derived/Drawer.h>
#include <NiUI/Derived/ProcHandler.h>
#include <memory>

class MyGame : public TakoFramework
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

    struct PostEffectParam
    {
        float vignettePower;
        float vignetteRange;
        float bloomThreshold;
        float bloomIntensity;
        float bloomSigma;
        Vector4 fogColor;
        float fogDensity;
    };

private: // メンバ変数

    PostEffectParam postEffectParam;

    bool FPSWindowVisible = true;
    bool PostEffectWindowVisible = false;
    
    std::unique_ptr<Drawer> drawer_ = nullptr;
    std::unique_ptr<ProcHandler> procHandler_ = nullptr;

    enum PostEffectType
    {
        NoEffect,
        VignetteRed,
        VignetteRedBloom,
        GrayScale,
        VigRedGrayScale,
        Bloom,
        BloomFog,
    };

    PostEffectType postEffectType = NoEffect;
};