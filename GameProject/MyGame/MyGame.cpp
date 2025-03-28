#include "MyGame.h"
#include "Audio.h"
#include "Input.h"
#include "Factory/SceneFactory.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "Draw2D.h"
#include "Object3dBasic.h"
#include "PostEffect.h"
#include "FrameTimer.h"
#include "GlobalVariables.h"
#include "Vector4.h"
#include <NiGui.h>
#include <GameSystem/StageManager/StageManager.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>



void MyGame::Initialize()
{
    winApp_->SetWindowSize(1600, 900);
    
    TakoFramework::Initialize();

#pragma region 汎用機能初期化-------------------------------------------------------------------------------------------------------------------
    // 入力クラスの初期化
    Input::GetInstance()->Initialize(winApp_);

    // オーディオの初期化
    Audio::GetInstance()->Initialize("resources/Sound/");

#pragma endregion

    // シーンの初期化
    sceneFactory_ = new SceneFactory();
    SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
    SceneManager::GetInstance()->ChangeScene("title", 0.0f);

    // PostEffectParamの設定
    postEffectParam.vignettePower = 0.f;
    postEffectParam.vignetteRange = 20.0f;
    postEffectParam.bloomThreshold = 1.0f;
    postEffectParam.bloomIntensity = 1.0f;
    postEffectParam.bloomSigma = 2.0f;
    postEffectParam.fogColor = {1.0f, 1.0f, 1.0f, 1.0f};
    postEffectParam.fogDensity = 0.01f;

    // テーマ編集
    OverrideImGuiStyle();

    // デルタタイムマネージャの初期化
    DeltaTimeManager::GetInstance()->SetDeltaTime(0, 1.0f / 60.0f);

    /// UIの初期化
    NiGui::Initialize({ WinApp::clientWidth, WinApp::clientHeight });

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

    /// Threadpoolの初期化
    threadpool_ = Threadpool::GetInstance();
    threadpool_->Initialize(3);
}

void MyGame::Finalize()
{
    TakoFramework::Finalize();

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
    imguiManager_->Begin();
    #endif // _DEBUG

    // 入力情報の更新
    Input::GetInstance()->Update();

    // UIの更新
    NiGui::BeginFrame();

    TakoFramework::Update();

    //　サウンドの更新
    Audio::GetInstance()->Update();

    // ゲームパッドの状態をリフレッシュ
    Input::GetInstance()->RefreshGamePadState();

    eventTimer_->EndEvent("Update");
}

void MyGame::Draw()
{
    /// ============================================= ///
    /// ------------------シーン描画-------------------///
    /// ============================================= ///

    eventTimer_->BeginEvent("Draw");

    // 描画前の処理(レンダーテクスチャを描画対象に設定)
    dx12_->SetRenderTexture();

    // テクスチャ用のsrvヒープの設定
    SrvManager::GetInstance()->BeginDraw();

    // シーンの描画
    SceneManager::GetInstance()->Draw();

    #ifdef _DEBUG
    NiGui::DrawDebug();
    #endif // _DEBUG

    // UIの描画
    NiGui::DrawUI();

    ParticleManager::GetInstance()->Draw();

    /// ===================================================== ///
    /// ------------------ポストエフェクト描画-------------------///
    /// ===================================================== ///
    // SwapChainを描画対象に設定
    dx12_->SetSwapChain();

    // PostEffectの描画
    switch (postEffectType)
    {
    case::MyGame::NoEffect:
        PostEffect::GetInstance()->Draw("NoEffect");
        break;
    case::MyGame::VignetteRed:
        PostEffect::GetInstance()->Draw("VignetteRed");
        break;
    case::MyGame::VignetteRedBloom:
        PostEffect::GetInstance()->Draw("VignetteRedBloom");
        break;
    case::MyGame::GrayScale:
        PostEffect::GetInstance()->Draw("GrayScale");
        break;
    case::MyGame::VigRedGrayScale:
        PostEffect::GetInstance()->Draw("VigRedGrayScale");
        break;
    case::MyGame::Bloom:
        PostEffect::GetInstance()->Draw("Bloom");
        break;
    case::MyGame::BloomFog:
        PostEffect::GetInstance()->Draw("BloomFog");
        break;
    }

    eventTimer_->EndEvent("Draw");

    eventTimer_->EndFrame();

    /// ========================================= ///
    ///-------------------ImGui-------------------///
    /// ========================================= ///
#ifdef _DEBUG

    eventTimer_->ImGui();

    SceneManager::GetInstance()->DrawImGui();

    Draw2D::GetInstance()->ImGui();

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
        ImGui::RadioButton("BloomFog", (int*)&postEffectType, BloomFog);

        ImGui::EndTabItem();
      }

      //ImGui::Separator();
      if (ImGui::BeginTabItem("PostEffect"))
      {
        if (postEffectType == VignetteRed || postEffectType == VignetteRedBloom || postEffectType == VigRedGrayScale)
        {
          ImGui::DragFloat("VignettePower", &postEffectParam.vignettePower, 0.01f, 0.0f, 10.0f);
          PostEffect::GetInstance()->SetVignettePower(postEffectParam.vignettePower);
          ImGui::DragFloat("VignetteRange", &postEffectParam.vignetteRange, 0.01f, 0.0f, 100.0f);
          PostEffect::GetInstance()->SetVignetteRange(postEffectParam.vignetteRange);
        }

        if (postEffectType == VignetteRedBloom)
        {
          ImGui::DragFloat("BloomThreshold", &postEffectParam.bloomThreshold, 0.01f, 0.0f, 1.0f);
          PostEffect::GetInstance()->SetBloomThreshold(postEffectParam.bloomThreshold);
        }

        if (postEffectType == Bloom || postEffectType == BloomFog)
        {
          ImGui::DragFloat("BloomIntensity", &postEffectParam.bloomIntensity, 0.01f, 0.0f, 10.0f);
          PostEffect::GetInstance()->SetBloomIntensity(postEffectParam.bloomIntensity);
          ImGui::DragFloat("BloomThreshold", &postEffectParam.bloomThreshold, 0.01f, 0.0f, 1.0f);
          PostEffect::GetInstance()->SetBloomThreshold(postEffectParam.bloomThreshold);
          ImGui::DragFloat("BloomSigma", &postEffectParam.bloomSigma, 0.01f, 0.0f, 10.0f);
          PostEffect::GetInstance()->SetBloomSigma(postEffectParam.bloomSigma);
        }

        if (postEffectType == BloomFog)
        {
          ImGui::ColorEdit4("FogColor", &postEffectParam.fogColor.x);
          PostEffect::GetInstance()->SetFogColor(postEffectParam.fogColor);
          ImGui::DragFloat("FogDensity", &postEffectParam.fogDensity, 0.01f, 0.0f, 1.0f);
          PostEffect::GetInstance()->SetFogDensity(postEffectParam.fogDensity);
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
