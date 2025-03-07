#include "../NiUI.h"

NiUI_ButtonState NiUI::Button(
    const std::string& _id,
    const std::string& _textureName,
    const NiVec2& _position,
    const NiVec2& _size,
    NiUI_StandardPoint _anchor,
    NiUI_StandardPoint _pivot)
{
    auto& buttonImage = buttonImages_[_id];
    bool isTrigger = false;
    bool isHover = false;
    bool isRelease = false;
    bool isHeld = false;

    NiVec2 posInRegion = _position;
    NiVec2 size = size_;
    if (state_.buffer.currentRegion != nullptr)
    {
        posInRegion = _position + state_.buffer.currentRegion->leftTop;
        size = state_.buffer.currentRegion->size;
    }

    auto leftTop = ComputeLeftTop(posInRegion, _size, size, _anchor, _pivot);

    // 当たり判定
    JudgeClickRect(leftTop, _size, isHover, isTrigger, isRelease);

    // ボタンの挙動
    bool onButton = ButtonBehavior(_id, isHover, isTrigger, isRelease, isHeld);

    /// ボタンのデータを更新
    buttonImage.id = _id;
    buttonImage.textureName = _textureName;
    buttonImage.leftTop = leftTop;
    buttonImage.size = _size;
    buttonImage.zOrder = state_.buffer.currentZOrder++;

    NiUI_ButtonState result = NiUI_ButtonState::None;
    if (onButton)
    {
        result = NiUI_ButtonState::Confirm;
    }
    else if (isHover)
    {
        result = NiUI_ButtonState::Hover;
    }

    return result;
}

bool NiUI::ButtonBehavior(const std::string& _id, bool _isHover, bool _isTrigger, bool _isRelease, bool& _out_held)
{
    if (_isHover)
    {
        state_.componentID.type = "Button";
        state_.componentID.hover = _id;
    }


    if(_isTrigger)
    {
        state_.componentID.type = "Button";
        state_.componentID.active = _id;
    }


    if(state_.componentID.active == _id)
    {
        _out_held = true;

        if (_isRelease && _isHover)
        {
            state_.componentID.active = {};
            return true;
        }
        else if(_isRelease)
        {
            state_.componentID.active = {};
        }
    }

    return false;
}

void NiUI::ButtonDataEnqueue()
{
    for(auto& buttonImage : buttonImages_)
    {
        // 描画クラスにデータを追加
        drawer_->EnqueueDrawInfo(&buttonImage.second);
    }
}