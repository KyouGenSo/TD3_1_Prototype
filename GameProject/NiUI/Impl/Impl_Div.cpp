#include "../NiUI.h"

#include <stdexcept> // runtime_error


bool NiUI::BeginDiv(const std::string& _id, const NiVec2& _position, const NiVec2& _size, const NiUI_StandardPoint _anchor, const NiUI_StandardPoint _pivot)
{
    // ネストカウントを増やす
    state_.valid.nestCount++;

    NiVec2 leftTop = {};
    NiVec2 size = {};
    NiVec2 parentPos = {};
    NiVec2 parentSize = {};

    /// 親の座標とサイズを取得
    if (state_.buffer.currentRegion == nullptr)
    {
        parentSize = size_;
    }
    else
    {
        parentPos = state_.buffer.currentRegion->leftTop;
        parentSize = state_.buffer.currentRegion->size;
    }

    /// サイズが親より大きい場合は親のサイズに合わせる
    size = _size;
    if (_size.x > parentSize.x) size.x = parentSize.x;
    if (_size.y > parentSize.y) size.y = parentSize.y;

    /// 親の座標を考慮した座標を計算
    NiVec2 posInRegion = _position;
    if (state_.buffer.currentRegion != nullptr)
    {
        posInRegion = _position + state_.buffer.currentRegion->leftTop;
    }

    /// ピボットとアンカーを考慮した座標を計算
    leftTop = ComputeLeftTop(posInRegion, size, parentSize, _anchor, _pivot);

    /// 親の更新と現在のリージョンの更新
    auto& divData = divData_[_id];
    divData.id = _id;
    divData.parent = state_.buffer.currentRegion;
    state_.buffer.currentRegion = dynamic_cast<BaseRegionData*>(&divData);

    /// =============
    /// 当たり判定と挙動

    bool isTrigger = false;
    bool isHover = false;
    bool isRelease = false;

    auto& leftTopDiff = regionLeftTopDifference_[_id];

    /// 範囲clamp
    NiVec2 newLeftTop = leftTop + leftTopDiff;
    if (state_.buffer.currentRegion != nullptr)
    {
        ClampRect(newLeftTop, size, parentPos, parentSize);
    }

    /// 当たり判定
    JudgeClickRect(newLeftTop, _size, isHover, isTrigger, isRelease);

    /// 挙動
    DivBehavior(_id, isHover, isTrigger, isRelease);

    /// データの更新
    divData.leftTop = newLeftTop;
    divData.size = _size;


    return true;
}

bool NiUI::BeginDivMovable(const std::string& _id, const NiVec2& _position, const NiVec2& _size, const NiUI_StandardPoint _anchor, const NiUI_StandardPoint _pivot)
{
    // ネストカウントを増やす
    state_.valid.nestCount++;

    NiVec2 leftTop = {};
    NiVec2 size = {};
    NiVec2 parentPos = {};
    NiVec2 parentSize = {};

    /// 親の座標とサイズを取得
    if (state_.buffer.currentRegion == nullptr)
    {
        parentSize = size_;
    }
    else
    {
        parentPos = state_.buffer.currentRegion->leftTop;
        parentSize = state_.buffer.currentRegion->size;
    }

    /// サイズが親より大きい場合は親のサイズに合わせる
    size = _size;
    if (_size.x > parentSize.x) size.x = parentSize.x;
    if (_size.y > parentSize.y) size.y = parentSize.y;

    /// 親の座標を考慮した座標を計算
    NiVec2 posInRegion = _position;
    if (state_.buffer.currentRegion != nullptr)
    {
        posInRegion = _position + state_.buffer.currentRegion->leftTop;
    }
    
    /// ピボットとアンカーを考慮した座標を計算
    leftTop = ComputeLeftTop(posInRegion, size, parentSize, _anchor, _pivot);

    /// 親の更新と現在のリージョンの更新
    auto& divData = divData_[_id];
    divData.id = _id;
    divData.parent = state_.buffer.currentRegion;
    state_.buffer.currentRegion = dynamic_cast<BaseRegionData*>(&divData);

    /// =============
    /// 当たり判定と挙動
    
    bool isTrigger = false;
    bool isHover = false;
    bool isRelease = false;

    auto& leftTopDiff = regionLeftTopDifference_[_id];

    /// 範囲clamp
    NiVec2 newLeftTop = leftTop + leftTopDiff;
    if (state_.buffer.currentRegion != nullptr)
    {
        ClampRect(newLeftTop, size, parentPos, parentSize);
    }

    /// 当たり判定
    JudgeClickRect(newLeftTop, _size, isHover, isTrigger, isRelease);

    /// 挙動
    DivBehaviorMovable(_id, isHover, isTrigger, isRelease);

    /// データの更新
    divData.leftTop = newLeftTop;
    divData.size = _size;


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
    if (_isHover)
    {
        state_.componentID.hover = _id;
        state_.componentID.type = "Div";
    }

    if (_isTrigger)
    {
        state_.componentID.active = _id;
    }

    if (_isRelease && state_.componentID.active == _id)
    {
        state_.componentID.active = {};
    }
}

void NiUI::DivBehaviorMovable(const std::string& _id, bool _isHover, bool _isTrigger, bool _isRelease)
{
    auto& regionDiff = regionLeftTopDifference_[_id];

    DivBehavior(_id, _isHover, _isTrigger, _isRelease);

    if (state_.componentID.active == _id)
    {
        regionDiff += io_.input.differencePos;
    }

    return;
}

void NiUI::DivDataEnqueue()
{
    for (auto& divData : divData_)
    {
        // 描画クラスにデータを追加
        drawer_->EnqueueDrawInfo(&divData.second);
    }
}
