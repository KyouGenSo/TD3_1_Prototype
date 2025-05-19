#include "Status.h"

#include <imgui.h>

void Status::Initalize()
{
    /// インスタンス取得
    pRandGen_ = RandomGenerator::GetInstance();

    hp_ = 0;
    maxHp_ = 0;
    attack_ = 0;
    defence_ = 0;
    speed_ = 0;
    xpAmount_ = 0;
}

void Status::Update()
{
    // 整合性チェック
    if (hp_ > maxHp_)
    {
        hp_ = maxHp_;
    }
}

void Status::ImGui(const std::string& _name)
{
    if (ImGui::Begin(("[Status] " + _name).c_str()))
    {
        ImGui::Text("HP: %.1f / %.1f", hp_, maxHp_);
        ImGui::Text("Attack: %.1f", attack_);
        ImGui::Text("Defence: %.1f", defence_);
        ImGui::Text("Speed: %.1f", speed_);
        ImGui::Text("XPAmount: %.1f", xpAmount_);
    }

    ImGui::End();
}

void Status::LoadFromFile(const std::string& _filename)
{

}

float Status::getExperiencePoints() const
{
    return pRandGen_->Generate(1.0f, xpAmount_);
}

float Status::getGainedXP() const
{
    return gainedXP_;
}

void Status::OnCollision(const Status& _status)
{
    ///=================
    ///=== Calc ========
    ///=================

    // ダメージ
    float damage = _status.attack_ - defence_;
    if (damage < 0.0f)
    {
        damage = 0.0f;
    }

    // 経験値
    float gainXP = _status.getExperiencePoints();
    gainedXP_ += gainXP;

    ///=================
    ///=== Apply =======
    ///=================

    // ダメージ
    hp_ -= damage;
    if (hp_ < 0.0f)
    {
        hp_ = 0.0f;
    }

    // 経験値
    
}
