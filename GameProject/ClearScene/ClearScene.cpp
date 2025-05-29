#include "ClearScene.h"

#include "Draw2D.h"
#include "EmitterManager.h"
#include "GPUParticle.h"
#include "Input.h"
#include "Object3dBasic.h"
#include "PostEffect.h"
#include "SceneManager.h"
#include "SpriteBasic.h"

#ifdef _DEBUG
#include "ImGui.h"
#endif

void ClearScene::Initialize()
{
    /// ================================== ///
    ///              初期化処理              ///
    /// ================================== ///

        // デフォルトカメラを生成
    camera_ = new Camera();
    camera_->SetRotate(Vector3(0.2f, 0.0f, 0.0f));
    camera_->SetTranslate(Vector3(0.0f, 9.0f, -34.0f));

    GPUParticle* particleSystem = GPUParticle::GetInstance();

    particleSystem->SetCamera(camera_);

    emitterManager_ = new EmitterManager(particleSystem);

    emitterParams_ = {
        .position = { 0.0f, -7.5f, 0.0f },
        .size = { 200.0f, 1.0f, 1.0f },
        .rotation = { 0.0f, 0.0f, 0.0f },
        .count = 20,
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
                .intensity = 1.8f,
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

    pressSize = { .x = 500, .y = 100 };
    pressAlpha = 0.f;
    pressAlphaIncreasing = true;
    pressAlphaSpeed = 0.02f;

    clearTextPos = { .x = 400.f, .y = 200.f };

    whiteBarVerPos = { .x = 80.f, .y = 0.f };
    whiteBarVerPos2 = { .x = 1520.f, .y = 0.f };

    whiteBarHorPos = { .x = 0.f, .y = 80.f };
    whiteBarHorPos2 = { .x = 0.f, .y = 880.f };

    whiteBarVerSize = { .x = 5.f, .y = 2000.f };
    whiteBarHorSize = { .x = 2000.f, .y = 5.f };

    bgSp_ = std::make_unique<Sprite>();
    bgSp_->Initialize("black.png");
    bgSp_->SetPos({ 0, 0 });

    clearText_ = std::make_unique<Sprite>();
    clearText_->Initialize("GameClear.png");
    clearText_->SetPos(clearTextPos);

    press_ = std::make_unique<Sprite>();
    press_->Initialize("press_space_text.png");
    press_->SetPos(Vector2(300, 600));


    whiteBarVer_ = std::make_unique<Sprite>();
    whiteBarVer_->Initialize("white.png");
    whiteBarVer_->SetPos(whiteBarVerPos);
    whiteBarVer_->SetSize(whiteBarVerSize);
    whiteBarVer2_ = std::make_unique<Sprite>();
    whiteBarVer2_->Initialize("white.png");
    whiteBarVer2_->SetPos(whiteBarVerPos2);
    whiteBarVer2_->SetSize(whiteBarVerSize);

    whiteBarHor_ = std::make_unique<Sprite>();
    whiteBarHor_->Initialize("white.png");
    whiteBarHor_->SetPos(whiteBarHorPos);
    whiteBarHor_->SetSize(whiteBarHorSize);
    whiteBarHor2_ = std::make_unique<Sprite>();
    whiteBarHor2_->Initialize("white.png");
    whiteBarHor2_->SetPos(whiteBarHorPos2);
    whiteBarHor2_->SetSize(whiteBarHorSize);

}


void ClearScene::Finalize()
{
    emitterManager_->RemoveAllEmitters();
    delete emitterManager_;
    emitterManager_ = nullptr;
}

void ClearScene::Update()
{
    /// ================================== ///
    ///              更新処理               ///
    /// ================================== ///

    camera_->Update();

    bgSp_->SetSize({ static_cast<float>(WinApp::clientWidth), static_cast<float>(WinApp::clientHeight) });
    bgSp_->Update();

    if (bloomParam_.sigmaIncreasing) {
        if (bloomParam_.sigma < 12.f) {
            bloomParam_.sigma += bloomParam_.sigmaIncreaSpeed;
        } else {
            bloomParam_.sigma = 12.f;
            bloomParam_.sigmaIncreasing = false;
        }
    } else {
        if (bloomParam_.sigma > 1.5f) {
            bloomParam_.sigma -= bloomParam_.sigmaIncreaSpeed * 0.5f;
        } else {
            bloomParam_.sigma = 1.5f;
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

    clearTextPos.x = WinApp::clientWidth / 2 - clearText_->GetSize().x / 2;
    clearTextPos.y = clearText_->GetSize().y + 100.f;
    clearText_->SetPos(clearTextPos);
    clearText_->Update();

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
        SceneManager::GetInstance()->ChangeScene("title");
    }
}

void ClearScene::Draw()
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

void ClearScene::DrawWithoutEffect()
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

    clearText_->Draw();
    press_->Draw();

}

void ClearScene::DrawImGui()
{
#ifdef _DEBUG

    /// ================================== ///
    ///             ImGuiの描画              ///
    /// ================================== ///


#endif // _DEBUG
}