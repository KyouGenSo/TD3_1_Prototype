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

    PostEffect::GetInstance()->SetEffectType("Bloom");

    TextureManager::GetInstance()->LoadTexture("prot-title.png");
    TextureManager::GetInstance()->LoadTexture("press_space_text.png");
    TextureManager::GetInstance()->LoadTexture("guide.png");
    TextureManager::GetInstance()->LoadTexture("white.png");

    bgmPlayHandle_ = SoundManager::GetInstance()->Play("BGM_Title");

    frameCount_ = 0;

    button_ = std::make_unique<Sprite>();
    button_->Initialize("prot-title.png");

    press_ = make_unique<Sprite>();
    press_->Initialize("press_space_text.png");
    press_->SetPos(Vector2(300, 600));

    guide_ = make_unique<Sprite>();
    guide_->Initialize("guide.png");
    guide_->SetPos(Vector2(300, 50));
    guide_->SetSize({800, 450});

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

    button_->SetPos(start);
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

    press_->Update();

    whiteBarVer_->Update();
    whiteBarVer2_->Update();
    whiteBarVer_->SetPos(whiteBarVerPos);
    whiteBarVer_->SetSize(whiteBarVerSize);

    whiteBarHor_->Update();
    whiteBarHor2_->Update();
    whiteBarHor_->SetPos(whiteBarHorPos);
    whiteBarHor_->SetSize(whiteBarHorSize);

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
    // whiteBarVerのImGui
    ImGui::Text("whiteBarVer");
    ImGui::DragFloat2("whiteBarVerPos", &whiteBarVerPos.x, 1.0f, 0.0f, 1600.0f);
    ImGui::DragFloat2("whiteBarVerSize", &whiteBarVerSize.x, 1.0f, 0.0f, 1600.0f);
    ImGui::Separator();
    // whiteBarHorのImGui
    ImGui::Text("whiteBarHor");
    ImGui::DragFloat2("whiteBarHorPos", &whiteBarHorPos.x, 1.0f, 0.0f, 1600.0f);
    ImGui::DragFloat2("whiteBarHorSize", &whiteBarHorSize.x, 1.0f, 0.0f, 1600.0f);
    ImGui::End();

#endif // _DEBUG
}
