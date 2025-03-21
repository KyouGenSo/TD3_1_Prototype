#include "TitleScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Object3dBasic.h"
#include "SpriteBasic.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "Input.h"
#include "Draw2D.h"
#include "Camera.h"
#include "Audio.h"
#include "GlobalVariables.h"
#include "Ease/Ease.h"

#ifdef _DEBUG
#include"ImGui.h"
#include "DebugCamera.h"
#endif

void TitleScene::InitParticle()
{
}

void TitleScene::InitVariables()
{
}

void TitleScene::Finalize()
{
}

void TitleScene::Initialize()
{
#ifdef _DEBUG
	DebugCamera::GetInstance()->Initialize();
  Object3dBasic::GetInstance()->SetDebug(false);
  Draw2D::GetInstance()->SetDebug(false);
  ParticleManager::GetInstance()->SetIsDebug(false);
#endif

	/// ================================== ///
	///              初期化処理              ///
	/// ================================== ///
  TextureManager::GetInstance()->LoadTexture("prot-title.png");

    frameCount_ = 0;

    button_ = std::make_unique<Sprite>();
    button_->Initialize("prot-title.png");

    button_->SetPos(start);
}

void TitleScene::Update()
{
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_F1)) {
		Object3dBasic::GetInstance()->SetDebug(!Object3dBasic::GetInstance()->GetDebug());
		Draw2D::GetInstance()->SetDebug(!Draw2D::GetInstance()->GetDebug());
		ParticleManager::GetInstance()->SetIsDebug(!ParticleManager::GetInstance()->GetIsDebug());
		isDebug_ = !isDebug_;
	}

	if (isDebug_) {
		DebugCamera::GetInstance()->Update();
	}
#endif
	/// ================================== ///
	///              更新処理               ///
	/// ================================== ///

    if (frameCount_ < AnimationFrames) {
        float t = frameCount_ / AnimationFrames;
        float progress = Ease::In::Back(t);
        pos.x = start.x + (end.x - start.x) * progress;
        pos.y = start.y + (end.y - start.y) * progress;
        frameCount_++;
    }

    button_->SetPos(pos);
    button_->Update();

	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
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



	//--------------------------------------------------//


	//-------------------Modelの描画-------------------//
	// 3Dモデル共通描画設定
	Object3dBasic::GetInstance()->SetCommonRenderSetting();


	//------------------------------------------------//


	//------------------前景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();

    button_->Draw();

	//--------------------------------------------------//

	Draw2D::GetInstance()->DrawGrid(100.0f, 20.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

void TitleScene::DrawImGui()
{
#ifdef _DEBUG

	/// ================================== ///
	///             ImGuiの描画              ///
	/// ================================== ///


#endif // _DEBUG
}

void TitleScene::ApplyGlobalVariables()
{
}
