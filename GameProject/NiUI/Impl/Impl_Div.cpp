#include "../NiUI.h"

#include <stdexcept> // runtime_error


bool NiUI::BeginDiv(const std::string& _id, const std::string& _textureName, const NiVec4& _color, const NiVec2& _position, const NiVec2& _size, const NiUI_StandardPoint _anchor, const NiUI_StandardPoint _pivot)
{
    NiVec2 leftTop = _position;
    NiVec2 size = _size;
    NiVec2 parentPos = {};
    NiVec2 parentSize = {};

    ComputeRect(_id, leftTop, size, parentPos, parentSize, _anchor, _pivot);

    /// =============
    /// 当たり判定と挙動

    bool isTrigger = false;
    bool isHover = false;
    bool isRelease = false;

    /// 当たり判定
    JudgeClickRect(leftTop, size, isHover, isTrigger, isRelease);

    /// 挙動
    DivBehavior(_id, isHover, isTrigger, isRelease);

    /// データの更新
    auto& divData = divData_[_id];
    divData.id = _id;
    divData.textureName = _textureName;
    divData.color = _color;
    divData.leftTop = leftTop;
    divData.size = size;
    divData.zOrder = state_.buffer.currentZOrder++;
    divData.parent = state_.buffer.currentRegion;

    /// 現在のリージョンを更新
    state_.buffer.currentRegion = dynamic_cast<BaseRegionData*>(&divData);


    return true;
}

bool NiUI::BeginDivMovable(const std::string& _id, const std::string& _textureName, const NiVec4& _color, const NiVec2& _position, const NiVec2& _size, const NiUI_StandardPoint _anchor, const NiUI_StandardPoint _pivot)
{
    NiVec2 leftTop = _position;
    NiVec2 size = _size;
    NiVec2 parentPos = {};
    NiVec2 parentSize = {};

    ComputeRect(_id, leftTop, size, parentPos, parentSize, _anchor, _pivot);

    /// =============
    /// 当たり判定と挙動
    
    bool isTrigger = false;
    bool isHover = false;
    bool isRelease = false;

    NiVec2 posClamped = leftTop + divOffset_[_id];
    ClampRect(posClamped, _size, parentPos, parentSize);

    /// 当たり判定
    JudgeClickRect(posClamped, size, isHover, isTrigger, isRelease);

    /// 挙動
    auto& regionDiff = divOffset_[_id];

    DivBehavior(_id, isHover, isTrigger, isRelease);

    OffsetUpdate(_id, posClamped, leftTop, size, parentPos, parentSize);

    /// データの更新
    auto& divData = divData_[_id];
    divData.id = _id;
    divData.textureName = _textureName;
    divData.color = _color;
    divData.leftTop = posClamped;
    divData.size = size;
    divData.zOrder = state_.buffer.currentZOrder++;
    divData.parent = state_.buffer.currentRegion;

    /// 現在のリージョンを更新
    state_.buffer.currentRegion = dynamic_cast<BaseRegionData*>(&divData);

    return true;
}

void NiUI::EndDiv()
{
    if (state_.valid.nestCount == 0)
    {
        throw std::runtime_error("ネスト終了関数の呼び出し過多です。");
    }

    state_.valid.nestCount--;

    state_.buffer.currentRegion = state_.buffer.currentRegion->parent;
}

void NiUI::DivBehavior(const std::string& _id, bool _isHover, bool _isTrigger, bool _isRelease)
{
    // ネストカウントを増やす
    state_.valid.nestCount++;

    if (_isHover)
    {
        state_.componentID.hover = _id;
        state_.componentID.type = "Div";
    }

    if (_isTrigger)
    {
        state_.componentID.active = _id;
    }
}

void NiUI::DivDataEnqueue()
{
    for (auto& divData : divData_)
    {
        // 描画クラスにデータを追加
        drawer_->EnqueueDrawInfo(&divData.second);
    }
}

void NiUI::ComputeRect(const std::string& _id, NiVec2& _leftTop, NiVec2& _size, NiVec2& _parentPos, NiVec2& _parentSize, const NiUI_StandardPoint _anchor, const NiUI_StandardPoint _pivot)
{
    /// 親の座標とサイズを取得
    if (state_.buffer.currentRegion == nullptr)
    {
        _parentSize = size_;
    }
    else
    {
        _parentPos = state_.buffer.currentRegion->leftTop;
        _parentSize = state_.buffer.currentRegion->size;
    }

    /// 親の座標を考慮した座標を計算
    if (state_.buffer.currentRegion != nullptr)
    {
        _leftTop += state_.buffer.currentRegion->leftTop;
    }

    /// ピボットとアンカーを考慮した座標を計算
    _leftTop = ComputeLeftTop(_leftTop, _size, _parentSize, _anchor, _pivot);
}
