#include "../NiUI.h"

#include <stdexcept> // runtime_error

bool NiUI::BeginDiv(const std::string& _id, const NiVec2& _position, const NiVec2& _size, const NiUI_StandardPoint _anchor, const NiUI_StandardPoint _pivot)
{
    // ネストカウントを増やす
    state_.valid.nestCount++;

    NiVec2 leftTop = {};

    if (state_.buffer.currentRegion == nullptr)
    {
        leftTop = ComputeLeftTop(_position, _size, size_, _anchor, _pivot);
    }
    else
    {
        leftTop = ComputeLeftTop(_position, _size, state_.buffer.currentRegion->size, _anchor, _pivot);
    }

    /// 親の更新と現在のリージョンの更新
    auto& divData = divData_[_id];
    divData.id = _id;
    divData.parent = state_.buffer.currentRegion;
    state_.buffer.currentRegion = dynamic_cast<BaseRegionData*>(&divData);

    bool isTrigger = false;
    bool isHover = false;
    bool isRelease = false;

    auto& leftTopEditted = regionEditted_[_id];

    if (leftTopEditted == NiVec2())
    {
        leftTopEditted = leftTop;
    }

    JudgeClickRect(leftTopEditted, _size, isHover, isTrigger, isRelease);

    DivBehavior(_id, isHover, isTrigger, isRelease, leftTop);

    divData.leftTop = leftTopEditted;
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


NiVec2 NiUI::DivBehavior(const std::string& _id, bool _isHover, bool _isTrigger, bool _isRelease, const NiVec2& _leftTop)
{
    auto& regionEditted = regionEditted_[_id];

    if (_isHover)
    {
        state_.componentID.hover = _id;
        state_.componentID.type = "Div";
    }

    if (_isTrigger)
    {
        state_.componentID.active = _id;
    }

    if (state_.componentID.active == _id)
    {
        regionEditted += io_.input.differencePos;
    }

    if (_isRelease && state_.componentID.active == _id)
    {
        state_.componentID.active = {};
    }

    return _leftTop + regionEditted;
}

void NiUI::DivDataEnqueue()
{
    for (auto& divData : divData_)
    {
        // 描画クラスにデータを追加
        drawer_->EnqueueDrawInfo(&divData.second);
    }
}
