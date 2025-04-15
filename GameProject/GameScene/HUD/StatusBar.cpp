#include "StatusBar.h"
#include <TextureManager.h>
#include <imgui.h>

#define TO_VECTOR2(class) Vector2{(class).x, (class).y}
#define TO_VECTOR4(class) Vector4{(class).x, (class).y, (class).z, (class).w}

const NiVec4 StatusBar::COLOR_BAR_NORMAL = rgba(41, 98, 92, 255);
const NiVec4 StatusBar::COLOR_BAR_BG = rgba(139, 139, 139, 255);
const NiVec4 StatusBar::COLOR_BAR_LOW = rgba(135, 48, 49, 255);

const NiVec2 StatusBar::SPACING_HEAD_TO_DECO = { 0.0f, 10.0f };
const NiVec2 StatusBar::BAR_SIZE = { 512.0f, 16.0f };

void StatusBar::Initialize(const std::string& _nameTexturePath)
{
    nameTexturePath_ = _nameTexturePath;

    // テクスチャの読み込み
    auto* tm = TextureManager::GetInstance();
    tm->LoadTexture(nameTexturePath_);
    tm->LoadTexture(PATH_BAR);
    tm->LoadTexture(PATH_DECORATION);

    // スプライトの初期化
    bar_ = std::make_unique<Sprite>();
    bar_->Initialize(PATH_BAR);
    bar_->SetColor(TO_VECTOR4(COLOR_BAR_NORMAL));

    decoration_ = std::make_unique<Sprite>();
    decoration_->Initialize(PATH_DECORATION);

    name_ = std::make_unique<Sprite>();
    name_->Initialize(nameTexturePath_);

    background_ = std::make_unique<Sprite>();
    background_->Initialize(PATH_BAR);
    background_->SetColor(TO_VECTOR4(COLOR_BAR_BG));
}

void StatusBar::Update()
{
    this->UpdateTransform();

    //if (currentValue_ < maxValue_ * BORDER_DANGER)
    //{
    //    bar_->SetColor(COLOR_BAR_LOW);
    //}
    //else
    //{
    //    bar_->SetColor(COLOR_BAR_NORMAL);
    //}

    float t = currentValue_ - 0.1f / maxValue_ + 0.1f;
    if (t > 1.0f) t = 1.0f;
    if (t < 0.0f) t = 0.0f;

    NiVec4 color = {};
    color.Lerp(COLOR_BAR_LOW, COLOR_BAR_NORMAL, currentValue_ / maxValue_);
    bar_->SetColor(TO_VECTOR4(color));

    if (decoration_) decoration_->Update();
    if (bar_) bar_->Update();
    if (background_) background_->Update();
    if (name_) name_->Update();
}

void StatusBar::Draw2D()
{
    if (decoration_) decoration_->Draw();
    if (background_) background_->Draw();
    if (bar_) bar_->Draw();
    if (name_) name_->Draw();
    for (const auto& number : numbers_)
    {
        if (number.second) number.second->Draw();
    }
}

void StatusBar::ImGui()
{
    if (ImGui::Begin(nameTexturePath_.c_str()))
    {
        ImGui::Text("Position");
        ImGui::DragFloat2("Position", &position_.x, 0.1f);
        ImGui::Text("Max Value");
        ImGui::DragFloat("Max Value", &maxValue_, 0.1f, FLT_MIN);
        ImGui::Text("Current Value");
        ImGui::DragFloat("Current Value", &currentValue_, 0.1f, FLT_MIN, maxValue_);
    }
    ImGui::End();
}

void StatusBar::UpdateTransform()
{
    NiVec2 leftTop = position_ - anchor_ * size_;
    NiVec2 cPos = leftTop;

    name_->SetPos(TO_VECTOR2(cPos));

    cPos.y += name_->GetSize().y;
    cPos += SPACING_HEAD_TO_DECO;
    decoration_->SetPos(TO_VECTOR2(cPos));

    NiVec2 decoSize = { decoration_->GetSize().x, decoration_->GetSize().y };

    cPos += (decoSize - BAR_SIZE) * 0.5f;
    bar_->SetPos(TO_VECTOR2(cPos));
    float ratio = currentValue_ / maxValue_;
    bar_->SetSize({ BAR_SIZE.x * ratio, bar_->GetSize().y });

    background_->SetPos(TO_VECTOR2(cPos));
    background_->SetSize(TO_VECTOR2(BAR_SIZE));
}
