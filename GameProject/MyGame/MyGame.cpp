#include "MyGame.h"
#include "Audio.h"
#include "Input.h"
#include "Factory/SceneFactory.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Draw2D.h"
#include "Object3dBasic.h"
#include "PostEffect.h"
#include "FrameTimer.h"
#include "GlobalVariables.h"
#include "Vector4.h"
#include <NiGui.h>
#include <GameSystem/StageManager/StageManager.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>
#include <GameSystem/SoundManager/SoundManager.h>
#include <GameUI/ColorResolver/ColorResolver.h>
#include <SpriteBasic.h>
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>

#include <Utility/RandomGenerator/RandomGenerator.h>

#include "GPUParticle.h"
#include "ModelManager.h"
#include <functional>

#include "Transition.h"


void MyGame::Initialize()
{
    winApp_->SetWindowSize(1600, 900);

    TakoFramework::Initialize();

#pragma region General functions initialization

    // 入力クラスの初期化
    Input::GetInstance()->Initialize(winApp_);

    // オーディオの初期化
    Audio::GetInstance()->Initialize("resources/Sound/");

#pragma endregion

#ifdef _DEBUG
    // PostEffectParamの設定
    postEffectParam.vignettePower = 0.f;
    postEffectParam.vignetteRange = 20.0f;
    postEffectParam.bloomThreshold = 1.0f;
    postEffectParam.bloomIntensity = 1.0f;
    postEffectParam.bloomSigma = 2.0f;
    postEffectParam.bloomKernelSize = 10;
    postEffectParam.bloomSampleCount = 10;
    postEffectParam.downSampleFactor = 8;
    postEffectParam.fogColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    postEffectParam.fogDensity = 0.01f;
#endif // _DEBUG

    // 乱数生成クラスの初期化
    RandomGenerator::Initialize();

    // シーンの初期化
    sceneFactory_ = new SceneFactory();
    SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
    SceneManager::GetInstance()->ChangeScene("title", 0.0f);

    // テーマ編集
    OverrideImGuiStyle();

    // デルタタイムマネージャの初期化
    DeltaTimeManager::GetInstance()->SetDeltaTime(0, 1.0f / 60.0f);
    DeltaTimeManager::GetInstance()->SetDeltaTime(1, 1.0f / 60.0f);

    /// UIの初期化
    NiGui::Initialize({ WinApp::clientWidth, WinApp::clientHeight });
    NiGui::SetClientSize({ WinApp::clientWidth, WinApp::clientHeight });

    /// 描画クラスの設定
    drawer_ = std::make_unique<NiGuiDrawer>();
    NiGui::SetDrawer(drawer_.get());

    /// ウィンドウプロシージャハンドラの設定
    procHandler_ = std::make_unique<ProcHandler>();
    winApp_->SetWndProcHandler(procHandler_.get());

    /// UIサウンドの設定
    NiGui::SetHoverSound(Audio::GetInstance()->LoadWaveFile("ui_hover.wav"));
    NiGui::SetConfirmSound(Audio::GetInstance()->LoadWaveFile("ui_confirm.wav"));

    /// デバッグUIの設定
    niguiDebug_ = std::make_unique<NiGuiDebug>();
    niguiDebug_->SetIO(&NiGui::GetIO());
    niguiDebug_->SetState(&NiGui::GetState());
    niguiDebug_->SetSetting(&NiGui::GetSetting());
    NiGui::SetDebug(niguiDebug_.get());

    /// StageManagerの初期化
    StageManager::GetInstance()->Initialize();
    StageManager::GetInstance()->SetCurrent("Stage1");

    /// EventTimerの初期化
    eventTimer_ = EventTimer::GetInstance();

    TextureManager::GetInstance()->LoadTexture("circle.png");
    TextureManager::GetInstance()->LoadTexture("cross.png");
    TextureManager::GetInstance()->LoadTexture("white.png");

    GPUParticle::GetInstance()->Initialize(dx12_, defaultCamera_);

    SoundManager::GetInstance()->Initialize("Sounds.json");

    ColorResolver::GetInstance()->Initialize()->LoadColorData("Color.json");


    handle_onresizes_.push_back(
        winApp_->RegisterOnResizeFunc(std::bind(&SpriteBasic::OnResize, SpriteBasic::GetInstance(), std::placeholders::_1))
    );
    handle_onExit_ = GameEventNotifier::GetInstance()->RegisterCallback("Exit", [this](auto) { endFlag_ = true; });
}

void MyGame::Finalize()
{
    for (auto& handle : handle_onresizes_)
    {
        winApp_->UnregisterOnResizeFunc(handle);
    }

    GameEventNotifier::GetInstance()->UnregisterCallback("Exit", handle_onExit_);

    TakoFramework::Finalize();

    GPUParticle::GetInstance()->Finalize();

    SoundManager::GetInstance()->Finalize();

    // Audioの解放
    Audio::GetInstance()->Finalize();

    // 入力クラスの解放
    Input::GetInstance()->Finalize();
}

void MyGame::Update()
{
    eventTimer_->NewFrame();

    eventTimer_->BeginEvent("Update");

#ifdef _DEBUG
    switch (postEffectType)
    {
    case NoEffect:
        PostEffect::GetInstance()->SetEffectType("NoEffect");
        break;
    case VignetteRed:
        PostEffect::GetInstance()->SetEffectType("VignetteRed");
        break;
    case VignetteRedBloom:
        PostEffect::GetInstance()->SetEffectType("VignetteRedBloom");
        break;
    case GrayScale:
        PostEffect::GetInstance()->SetEffectType("GrayScale");
        break;
    case VigRedGrayScale:
        PostEffect::GetInstance()->SetEffectType("VigRedGrayScale");
        break;
    case Bloom:
        PostEffect::GetInstance()->SetEffectType("Bloom");
        break;
    case NewBloom:
        PostEffect::GetInstance()->SetEffectType("NewBloom");
        break;
    case BloomFog:
        PostEffect::GetInstance()->SetEffectType("BloomFog");
        break;
    case RadialBlur:
        PostEffect::GetInstance()->SetEffectType("RadialBlur");
        break;
    }
    PostEffect::GetInstance()->SetVignettePower(postEffectParam.vignettePower);
    PostEffect::GetInstance()->SetVignetteRange(postEffectParam.vignetteRange);
    //PostEffect::GetInstance()->SetBloomThreshold(postEffectParam.bloomThreshold);
    //PostEffect::GetInstance()->SetBloomIntensity(postEffectParam.bloomIntensity);
    //PostEffect::GetInstance()->SetBloomThreshold(postEffectParam.bloomThreshold);
    //PostEffect::GetInstance()->SetBloomSigma(postEffectParam.bloomSigma);
    //PostEffect::GetInstance()->SetBloomKernelSize(postEffectParam.bloomKernelSize);
    PostEffect::GetInstance()->SetFogColor(postEffectParam.fogColor);
    PostEffect::GetInstance()->SetFogDensity(postEffectParam.fogDensity);
    PostEffect::GetInstance()->SetRadialBlurCenter(postEffectParam.radialBlurCenter);
    PostEffect::GetInstance()->SetRadialBlurWidth(postEffectParam.radialBlurWidth);
    PostEffect::GetInstance()->SetBloomSampleCount(postEffectParam.radialBlurSampleCount);
    PostEffect::GetInstance()->SetBloomSampleCount(postEffectParam.bloomSampleCount);
#endif // _DEBUG

    // 入力情報の更新
    Input::GetInstance()->Update();

    // カメラの更新
    defaultCamera_->Update();

    if (Input::GetInstance()->TriggerKey(DIK_F11))
    {
        // フルスクリーンの切り替え
        ToggleFullScreen();
        NiGui::SetWindowInfo({ WinApp::clientWidth, WinApp::clientHeight }, {});
        NiGui::SetClientSize({ WinApp::clientWidth, WinApp::clientHeight });
    }

#ifdef _DEBUG
    imguiManager_->Begin();
#endif // _DEBUG

    // UIの更新
    NiGui::BeginFrame();

    GPUParticle::GetInstance()->Update();

    TakoFramework::Update();

    //　サウンドの更新
    Audio::GetInstance()->Update();

    // ゲームパッドの状態をリフレッシュ
    Input::GetInstance()->RefreshGamePadState();

    eventTimer_->EndEvent("Update");
}

void MyGame::Draw()
{
    eventTimer_->BeginEvent("Draw");

    /// ============================================= ///
    /// ------------------シーン描画-------------------///
    /// ============================================= ///

    //ポストエフェクト適用対象のレンダーテクスチャを描画先に設定
    dx12_->SetEffectRenderTexture();

    // テクスチャ用のsrvヒープの設定
    SrvManager::GetInstance()->BeginDraw();

    // シーンの描画
    SceneManager::GetInstance()->Draw();

#ifdef _DEBUG
    NiGui::DrawDebug();
#endif // _DEBUG

    /// ===================================================== ///
    /// ------------------ポストエフェクト描画-------------------///
    /// ===================================================== ///

    // ポストエフェクトの描画
    PostEffect::GetInstance()->Draw();

    /// ===================================================== ///
    /// ------------ポストエフェクト非適用対象の描画---------------///
    /// ===================================================== ///
    // ポストエフェクト非適用対象のレンダーテクスチャを描画先に設定
    dx12_->SetNonEffectRenderTexture();

    // シーンの描画
    SceneManager::GetInstance()->DrawWithoutEffect();

    // UIの描画
    NiGui::DrawUI();

    Draw2D::GetInstance()->Draw();

    GPUParticle::GetInstance()->Draw();

    Transition::GetInstance()->Draw();

    Draw2D::GetInstance()->Reset();

    /// ============================================= ///
    /// ---------最終結果をスワップチェーンに描画---------///
    /// ============================================= ///
    PostEffect::GetInstance()->DrawFinalResult();

    eventTimer_->EndEvent("Draw");

    eventTimer_->EndFrame();

    /// ========================================= ///
    ///-------------------ImGui-------------------///
    /// ========================================= ///
#ifdef _DEBUG

    eventTimer_->ImGui();

    SceneManager::GetInstance()->DrawImGui();

    Draw2D::GetInstance()->ImGui();

    SoundManager::GetInstance()->ImGui();

    // GlobalVariablesの更新
    GlobalVariables::GetInstance()->Update();

    ImGui::Begin("Option");
    // buttonでFPSの表示を切り替え
    if (ImGui::Button("Display FPS"))
    {
        FPSWindowVisible = !FPSWindowVisible;
    }
    ImGui::SameLine();
    if (ImGui::Button("PostEffect Option"))
    {
        PostEffectWindowVisible = !PostEffectWindowVisible;
    }

    ImGui::End();

    // fpsの表示
    if (FPSWindowVisible)
    {
        ImGui::Begin("FPS", &FPSWindowVisible);
        ImGui::ProgressBar(FrameTimer::GetInstance()->GetFPS() / 60.0f, ImVec2(0.0f, 0.0f), "");
        ImGui::SameLine();
        ImGui::Text("FPS : %.0f", FrameTimer::GetInstance()->GetFPS());
        ImGui::End();
    }


    // PostEffectのパラメータ調整
    if (PostEffectWindowVisible) {
        ImGui::Begin("PostEffect", &PostEffectWindowVisible);
        if (ImGui::BeginTabBar("PostEffectTab"))
        {

            if (ImGui::BeginTabItem("PostEffectType"))
            {
                ImGui::RadioButton("NoEffect", (int*)&postEffectType, NoEffect);
                ImGui::RadioButton("VignetteRed", (int*)&postEffectType, VignetteRed);
                ImGui::RadioButton("VignetteRedBloom", (int*)&postEffectType, VignetteRedBloom);
                ImGui::RadioButton("GrayScale", (int*)&postEffectType, GrayScale);
                ImGui::RadioButton("VigRedGrayScale", (int*)&postEffectType, VigRedGrayScale);
                ImGui::RadioButton("Bloom", (int*)&postEffectType, Bloom);
                ImGui::RadioButton("NewBloom", (int*)&postEffectType, NewBloom);
                ImGui::RadioButton("BloomFog", (int*)&postEffectType, BloomFog);
                ImGui::RadioButton("RadialBlur", (int*)&postEffectType, RadialBlur);

                ImGui::EndTabItem();
            }

            //ImGui::Separator();
            if (ImGui::BeginTabItem("PostEffect"))
            {
                if (postEffectType == VignetteRed || postEffectType == VignetteRedBloom || postEffectType == VigRedGrayScale)
                {
                    ImGui::DragFloat("VignettePower", &postEffectParam.vignettePower, 0.01f, 0.0f, 10.0f);
                    ImGui::DragFloat("VignetteRange", &postEffectParam.vignetteRange, 0.01f, 0.0f, 100.0f);
                }

                if (postEffectType == VignetteRedBloom)
                {
                    ImGui::DragFloat("BloomThreshold", &postEffectParam.bloomThreshold, 0.01f, 0.0f, 1.0f);
                    
                }

                if (postEffectType == Bloom || postEffectType == BloomFog || postEffectType == NewBloom)
                {
                    ImGui::DragFloat("BloomIntensity", &postEffectParam.bloomIntensity, 0.01f, 0.0f, 10.0f);
                    ImGui::DragFloat("BloomThreshold", &postEffectParam.bloomThreshold, 0.01f, 0.0f, 1.0f);
                    ImGui::DragFloat("BloomSigma", &postEffectParam.bloomSigma, 0.01f, 0.1f, 50.0f);
                    ImGui::DragInt("BloomKernelSize", &postEffectParam.bloomKernelSize, 1, 1, 100);
                    
                }

                if (postEffectType == BloomFog)
                {
                    ImGui::ColorEdit4("FogColor", &postEffectParam.fogColor.x);
                    ImGui::DragFloat("FogDensity", &postEffectParam.fogDensity, 0.01f, 0.0f, 1.0f);
                }

                if (postEffectType == RadialBlur)
                {
                    ImGui::DragFloat2("RadialBlurCenter", &postEffectParam.radialBlurCenter.x, 0.01f, 0.0f, 1.0f);
                    ImGui::DragFloat("RadialBlurWidth", &postEffectParam.radialBlurWidth, 0.01f, 0.0f, 1.0f);
                    ImGui::DragInt("RadialBlurSampleCount", &postEffectParam.radialBlurSampleCount, 1.0f, 1, 100);
                }

                if (postEffectType == NewBloom)
                {
                    ImGui::DragInt("BloomSampleCount", &postEffectParam.bloomSampleCount, 1, 1, 100);
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }


        ImGui::End();
    }

    imguiManager_->End();

    //imguiの描画
    imguiManager_->Draw();
#endif


    // 描画後の処理
    dx12_->EndDraw();
}

void MyGame::OverrideImGuiStyle()
{
#ifdef _DEBUG
    auto& style = ImGui::GetStyle();
    style.FramePadding = ImVec2(3.0f, 3.4f);
    style.IndentSpacing = 22.0f;
#endif

}
