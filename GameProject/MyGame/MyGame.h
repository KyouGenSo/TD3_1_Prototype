#pragma once
#include"TakoFramework.h"
#include"Vector2.h"
#include <NiGui/Derived/Drawer.h>
#include <NiGui/Derived/ProcHandler.h>
#include <NiGui/Derived/NiGui_Debug.h>
#include <EventTimer/EventTimer.h>
#include <Utility/Threadpool/Threadpool.h>
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
        int bloomKernelSize;
        int bloomSampleCount;
        int downSampleFactor;

        Vector4 fogColor;
        float fogDensity;

        Vector2 radialBlurCenter;
        float radialBlurWidth;
        int32_t radialBlurSampleCount;

        float bwFilterThreshold;

        float rgbSplitIntensity;
        Vector2 redOffset;
        Vector2 blueOffset;
        Vector2 greenOffset;
    };

private: // メンバ変数

    std::unique_ptr<NiGuiDrawer> drawer_ = nullptr;
    std::unique_ptr<ProcHandler> procHandler_ = nullptr;
    std::unique_ptr<NiGuiDebug> niguiDebug_ = nullptr;
    std::vector<uint32_t> handle_onresizes_ = {};

    EventTimer* eventTimer_ = nullptr;

#ifdef _DEBUG
    PostEffectParam postEffectParam = {};


    enum PostEffectType
    {
        NoEffect,
        VignetteRed,
        VignetteRedBloom,
        GrayScale,
        VigRedGrayScale,
        Bloom,
        NewBloom,
        BloomFog,
        RadialBlur,
        BWFilter,
        RGBSplit,
    };

    PostEffectType postEffectType = NoEffect;

    bool FPSWindowVisible = true;
    bool PostEffectWindowVisible = false;
#endif // _DEBUG

private:
    void OverrideImGuiStyle();
};