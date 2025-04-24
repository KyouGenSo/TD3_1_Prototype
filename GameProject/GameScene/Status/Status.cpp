#include "Status.h"

#include <imgui.h>

void Status::Initalize()
{
    hp_ = 0;
    maxHp_ = 0;
    attack_ = 0;
    defence_ = 0;
    speed_ = 0;
    level_ = 0;
    exp_ = 0;
    maxExp_ = 0;
}

void Status::Update()
{
    // 整合性チェック
    if (hp_ > maxHp_)
    {
        hp_ = maxHp_;
    }
    if (exp_ > maxExp_)
    {
        exp_ = maxExp_;
    }
    if (level_ < 1.0f)
    {
        level_ = 1.0f;
    }

    // レベルアップ
    if (exp_ >= maxExp_)
    {
        level_++;
        exp_ -= maxExp_;
        maxExp_ = level_ * 100.0f;
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
        ImGui::Text("Level: %.1f", level_);
        ImGui::Text("Exp: %.1f / %.1f", exp_, maxExp_);
    }

    ImGui::End();
}

void Status::LoadFromFile(const std::string& _filename)
{

}

void Status::OnCollision(const Status& _status)
{
    // ダメージ計算
    float damage = _status.attack_ - defence_;
    if (damage < 0.0f)
    {
        damage = 0.0f;
    }

    // ダメージ適用
    hp_ -= damage;
    if (hp_ < 0.0f)
    {
        hp_ = 0.0f;
    }
}
