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
    if (level_ < 1)
    {
        level_ = 1;
    }

    // レベルアップ
    if (exp_ >= maxExp_)
    {
        level_++;
        exp_ -= maxExp_;
        maxExp_ = level_ * 100;
    }
}

void Status::ImGui(const std::string& _name)
{
    if (ImGui::Begin(("[Status] " + _name).c_str()))
    {
        ImGui::Text("HP: %d / %d", hp_, maxHp_);
        ImGui::Text("Attack: %d", attack_);
        ImGui::Text("Defence: %d", defence_);
        ImGui::Text("Speed: %d", speed_);
        ImGui::Text("Level: %d", level_);
        ImGui::Text("Exp: %d / %d", exp_, maxExp_);
    }

    ImGui::End();
}

void Status::OnCollision(const Status& _status)
{
    // ダメージ計算
    int damage = _status.attack_ - defence_;
    if (damage < 0)
    {
        damage = 0;
    }

    // ダメージ適用
    hp_ -= damage;
    if (hp_ < 0)
    {
        hp_ = 0;
    }
}
