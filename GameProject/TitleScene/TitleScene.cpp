#include "TitleScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Object3dBasic.h"
#include "SpriteBasic.h"
#include "Input.h"
#include "Draw2D.h"
#include "Audio.h"
#include "GPUParticle.h"
#include "PostEffect.h"
#include "Ease/Ease.h"

#include <GameSystem/SoundManager/SoundManager.h>

#ifdef _DEBUG
#include"ImGui.h"
#include "DebugCamera.h"
#endif

void TitleScene::Finalize()
{
    Audio::GetInstance()->StopWave(bgmPlayHandle_);
}

void TitleScene::Initialize()
{
#ifdef _DEBUG
    DebugCamera::GetInstance()->Initialize();
    Object3dBasic::GetInstance()->SetDebug(false);
    Draw2D::GetInstance()->SetDebug(false);
    GPUParticle::GetInstance()->SetIsDebug(false);
#endif

    /// ================================== ///
    ///              初期化処理              ///
    /// ================================== ///

    GPUParticle* particleSystem = GPUParticle::GetInstance();

    emitterManager_ = std::make_unique<EmitterManager>(particleSystem);

    emitterParams_ = {
        .position = { 0.0f, -7.5f, 0.0f },
        .size = { 200.0f, 1.0f, 1.0f },
        .rotation = { 0.0f, 0.0f, 0.0f },
        .count = 10,
        .frequency = 0.05f,
        .scaleRangeX = { 0.05f, 0.05f },
        .scaleRangeY = { 0.5f, 1.0f },
        .velRangeX = { 0.001f, 0.001f },
        .velRangeY = { 0.05f, 0.1f },
        .velRangeZ = { 0.001f, 0.001f },
        .lifeTimeRange = { 1.0f, 5.0f },
        .isActive = true,
    };
    emitterManager_->CreateBoxEmitter("bg", emitterParams_.position, emitterParams_.size, emitterParams_.rotation, emitterParams_.count, emitterParams_.frequency);
    emitterManager_->SetEmitterScaleRange("bg", emitterParams_.scaleRangeX, emitterParams_.scaleRangeY);
    emitterManager_->SetEmitterVelocityRange("bg", emitterParams_.velRangeX, emitterParams_.velRangeY, emitterParams_.velRangeZ);
    emitterManager_->SetEmitterColor("bg", Vector4(1.f, 1.f, 1.f, 1.0f));
    emitterManager_->SetEmitterLifeTimeRange("bg", emitterParams_.lifeTimeRange);
    emitterManager_->SetEmitterActive("bg", emitterParams_.isActive);

    bloomParam_ = {
                .intensity = 1.0f,
                .threshold = 0.9f,
                .sigma = 0.f,
                .sigmaIncreaSpeed = 0.2f,
                .sigmaIncreasing = false,
                .kernelSize = 50 };

    PostEffect::GetInstance()->SetEffectType("Bloom");
    PostEffect::GetInstance()->SetBloomIntensity(bloomParam_.intensity);
    PostEffect::GetInstance()->SetBloomThreshold(bloomParam_.threshold);
    PostEffect::GetInstance()->SetBloomSigma(bloomParam_.sigma);
    PostEffect::GetInstance()->SetBloomKernelSize(bloomParam_.kernelSize);

    TextureManager::GetInstance()->LoadTexture("prot-title.png");
    TextureManager::GetInstance()->LoadTexture("press_space_text.png");
    TextureManager::GetInstance()->LoadTexture("guide.png");
    TextureManager::GetInstance()->LoadTexture("white.png");

    bgmPlayHandle_ = SoundManager::GetInstance()->Play("BGM_Title");

    pressSize = { .x = 500, .y = 100 };
    pressAlpha = 0.f;
    pressAlphaIncreasing = true;
    pressAlphaSpeed = 0.02f;

    whiteBarVerPos = { .x = 80.f, .y = 0.f };
    whiteBarVerPos2 = { .x = 1520.f, .y = 0.f };

    whiteBarHorPos = { .x = 0.f, .y = 80.f };
    whiteBarHorPos2 = { .x = 0.f, .y = 880.f };

    whiteBarVerSize = { .x = 5.f, .y = 2000.f };
    whiteBarHorSize = { .x = 2000.f, .y = 5.f };

    bgSp_ = make_unique<Sprite>();
    bgSp_->Initialize("black.png");
    bgSp_->SetPos({ 0, 0 });

    button_ = std::make_unique<Sprite>();
    button_->Initialize("prot-title.png");

    press_ = make_unique<Sprite>();
    press_->Initialize("press_space_text.png");
    press_->SetPos(Vector2(300, 600));

    guide_ = make_unique<Sprite>();
    guide_->Initialize("guide.png");
    guide_->SetPos(Vector2(300, 50));
    guide_->SetSize({ 800, 450 });

    whiteBarVer_ = make_unique<Sprite>();
    whiteBarVer_->Initialize("white.png");
    whiteBarVer_->SetPos(whiteBarVerPos);
    whiteBarVer_->SetSize(whiteBarVerSize);
    whiteBarVer2_ = make_unique<Sprite>();
    whiteBarVer2_->Initialize("white.png");
    whiteBarVer2_->SetPos(whiteBarVerPos2);
    whiteBarVer2_->SetSize(whiteBarVerSize);

    whiteBarHor_ = make_unique<Sprite>();
    whiteBarHor_->Initialize("white.png");
    whiteBarHor_->SetPos(whiteBarHorPos);
    whiteBarHor_->SetSize(whiteBarHorSize);
    whiteBarHor2_ = make_unique<Sprite>();
    whiteBarHor2_->Initialize("white.png");
    whiteBarHor2_->SetPos(whiteBarHorPos2);
    whiteBarHor2_->SetSize(whiteBarHorSize);
}

void TitleScene::Update()
{
#ifdef _DEBUG
    if (Input::GetInstance()->TriggerKey(DIK_F1)) {
        Object3dBasic::GetInstance()->SetDebug(!Object3dBasic::GetInstance()->GetDebug());
        Draw2D::GetInstance()->SetDebug(!Draw2D::GetInstance()->GetDebug());
        GPUParticle::GetInstance()->SetIsDebug(!GPUParticle::GetInstance()->GetIsDebug());
        isDebug_ = !isDebug_;
    }

    if (isDebug_) {
        DebugCamera::GetInstance()->Update();
    }
#endif
    /// ================================== ///
    ///              更新処理               ///
    /// ================================== ///

    bgSp_->SetSize({ static_cast<float>(WinApp::clientWidth), static_cast<float>(WinApp::clientHeight) });
    bgSp_->Update();

    if (bloomParam_.sigmaIncreasing) {
        if (bloomParam_.sigma < 10.f) {
            bloomParam_.sigma += bloomParam_.sigmaIncreaSpeed;
        } else {
            bloomParam_.sigma = 10.f;
            bloomParam_.sigmaIncreasing = false;
        }
    } else {
        if (bloomParam_.sigma > 0.1f) {
            bloomParam_.sigma -= bloomParam_.sigmaIncreaSpeed * 0.5f;
        } else {
            bloomParam_.sigma = 0.1f;
            bloomParam_.sigmaIncreasing = true;
        }
    }

    PostEffect::GetInstance()->SetBloomSigma(bloomParam_.sigma);

    // pressのアルファ値の更新
    if (pressAlphaIncreasing) {
        pressAlpha += pressAlphaSpeed;
        if (pressAlpha > 1.f) {
            pressAlpha = 1.f;
            pressAlphaIncreasing = false;
        }
    } else {
        pressAlpha -= pressAlphaSpeed;
        if (pressAlpha < 0.f) {
            pressAlpha = 0.f;
            pressAlphaIncreasing = true;
        }
    }
    // pressのアルファ値を設定
    press_->SetAlpha(pressAlpha);

    pressPos.x = WinApp::clientWidth / 2 - pressSize.x / 2;
    pressPos.y = WinApp::clientHeight - pressSize.y - 135.f;
    press_->SetPos(pressPos);
    press_->Update();

    whiteBarVerPos.x = whiteBarVerSize.x + 80.f;
    whiteBarVer_->SetPos(whiteBarVerPos);
    whiteBarVer_->SetSize(whiteBarVerSize);
    whiteBarVer_->Update();

    whiteBarVerPos2.x = WinApp::clientWidth - whiteBarVerSize.x - 80.f;
    whiteBarVer2_->SetPos(whiteBarVerPos2);
    whiteBarVer2_->SetSize(whiteBarVerSize);
    whiteBarVer2_->Update();

    whiteBarHorPos.y = whiteBarHorSize.y + 80.f;
    whiteBarHor_->SetPos(whiteBarHorPos);
    whiteBarHor_->SetSize(whiteBarHorSize);
    whiteBarHor_->Update();

    whiteBarHorPos2.y = WinApp::clientHeight - whiteBarHorSize.y - 80.f;
    whiteBarHor2_->SetPos(whiteBarHorPos2);
    whiteBarHor2_->SetSize(whiteBarHorSize);
    whiteBarHor2_->Update();


    emitterManager_->Update();
    emitterManager_->UpdateBoxEmitter("bg", emitterParams_.position, emitterParams_.size, emitterParams_.rotation, emitterParams_.count, emitterParams_.frequency);
    emitterManager_->SetEmitterVelocityRange("bg", emitterParams_.velRangeX, emitterParams_.velRangeY, emitterParams_.velRangeZ);
    emitterManager_->SetEmitterScaleRange("bg", emitterParams_.scaleRangeX, emitterParams_.scaleRangeY);
    emitterManager_->SetEmitterLifeTimeRange("bg", emitterParams_.lifeTimeRange);
    emitterManager_->SetEmitterActive("bg", emitterParams_.isActive);

    if (Input::GetInstance()->TriggerKey(DIK_SPACE))
    {
        SceneManager::GetInstance()->ChangeScene("play");
    }
}

void TitleScene::Draw()
{
    /// ================================== ///
    ///              描画処理               ///
    /// ================================== ///
    //------------------背景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();

    bgSp_->Draw();


    //-------------------Modelの描画-------------------//
    // 3Dモデル共通描画設定
    Object3dBasic::GetInstance()->SetCommonRenderSetting();




    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();

    whiteBarVer_->Draw();
    whiteBarHor_->Draw();

    whiteBarVer2_->Draw();
    whiteBarHor2_->Draw();

}

void TitleScene::DrawWithoutEffect()
{
    /// ================================== ///
    ///              描画処理               ///
    /// ================================== ///
    //------------------背景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();



    //-------------------Modelの描画-------------------//
    // 3Dモデル共通描画設定
    Object3dBasic::GetInstance()->SetCommonRenderSetting();




    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();

    press_->Draw();
}

void TitleScene::DrawImGui()
{
#ifdef _DEBUG

    /// ================================== ///
    ///             ImGuiの描画              ///
    /// ================================== ///

    ImGui::Begin("TitleScene");
    // emitterManagerのImGui
    ImGui::DragFloat3("Emitter Position", &emitterParams_.position.x, 0.1f, -1000.f, 1000.f);
    ImGui::DragFloat3("Emitter Size", &emitterParams_.size.x, 0.1f, 0.f, 1000.f);
    ImGui::DragFloat3("Emitter Rotation", &emitterParams_.rotation.x, 0.1f, -3.14f, 3.14f);
    ImGui::DragInt("Emitter Count", reinterpret_cast<int*>(&emitterParams_.count), 1, 1, 20000);
    ImGui::DragFloat("Emitter Frequency", &emitterParams_.frequency, 0.01f, 0.f, 10.f);
    ImGui::DragFloat2("Emitter Scale Range X", &emitterParams_.scaleRangeX.x, 0.01f, 0.f, 10.f);
    ImGui::DragFloat2("Emitter Scale Range Y", &emitterParams_.scaleRangeY.x, 0.01f, 0.f, 10.f);
    ImGui::DragFloat2("Emitter Velocity Range X", &emitterParams_.velRangeX.x, 0.01f, -1000.f, 1000.f);
    ImGui::DragFloat2("Emitter Velocity Range Y", &emitterParams_.velRangeY.x, 0.01f, -1000.f, 1000.f);
    ImGui::DragFloat2("Emitter Velocity Range Z", &emitterParams_.velRangeZ.x, 0.01f, -1000.f, 1000.f);
    ImGui::DragFloat2("Emitter Life Time Range", &emitterParams_.lifeTimeRange.x, 0.01f, 0.f, 10.f);
    ImGui::Checkbox("Emitter Active", &emitterParams_.isActive);

    // buttonを押すと一時的なエミッターを生成
    if (ImGui::Button("Create Temporary Emitter"))
    {
        emitterManager_->CreateTemporaryEmitterFrom("bg", "temp", 1.f);
    }

    ImGui::End();

#endif // _DEBUG
}
