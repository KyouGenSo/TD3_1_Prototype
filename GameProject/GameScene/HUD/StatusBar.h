#pragma once
#include <memory>
#include <Sprite.h>
#include <vector>

#include <NiGui/NiVec4.h>
#include <Math/NiVec2.h>
#include <array>

#define rgba(r, g, b, a) NiVec4{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f}

class StatusBar
{
public:
    StatusBar() = default;
    ~StatusBar() = default;

    void Initialize(const std::string& _nameTexturePath, const NiVec2& _barSize);
    void Update();
    void Draw2D();
    void ImGui();

public:
    void SetPosition(const NiVec2& _pos) { position_ = _pos; }
    void SetAnchorPoint(const NiVec2& _anchor) { anchor_ = _anchor; }
    void SetMaxValue(float _max) { maxValue_ = _max; }
    void SetCurrentValue(float _current) { currentValue_ = _current; }

    void operator=(const float& _val) { currentValue_ = _val; }
    float& operator+=(const float& _val) { currentValue_ += _val; return currentValue_; }

private:
    using NumberImage = std::pair<uint32_t, std::unique_ptr<Sprite>>;

    static constexpr char PATH_BAR[] = "white.png";
    static constexpr char PATH_DECORATION[] = "white.png";

    static const NiVec4 COLOR_BAR_NORMAL;
    static const NiVec4 COLOR_BAR_LOW;
    static const NiVec4 COLOR_BAR_BG;

    static const NiVec2 SPACING_HEAD_TO_DECO;
    NiVec2 barSize_ = { 100.0f , 10.0f };

    static constexpr float BORDER_DANGER = 0.25f;

    std::unique_ptr<Sprite> bar_ = nullptr;
    std::unique_ptr<Sprite> name_ = nullptr;
    std::array<std::unique_ptr<Sprite>, 2> decorations_ = {};
    std::unique_ptr<Sprite> background_ = nullptr;
    std::vector<NumberImage> numbers_;

    std::string nameTexturePath_ = "";

    NiVec2 position_ = { 0.0f, 0.0f };
    NiVec2 size_ = { 0.0f, 0.0f };
    NiVec2 anchor_ = { 0.0f, 0.0f };

    float maxValue_ = 0.0f;
    float currentValue_ = 0.0f;

private:
    void UpdateTransform();
};