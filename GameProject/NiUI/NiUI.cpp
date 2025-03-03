#include "NiUI.h"

#include <stdexcept> // runtime_error


NiUI_Input          NiUI::input_        = NiUI_Input();
NiVec2              NiUI::leftTop_      = { 0, 0 };
NiVec2              NiUI::size_         = { 0, 0 };
IDrawer*            NiUI::drawer_       = nullptr;

NiUIIO              NiUI::io_           = NiUIIO();
NiUICoreState       NiUI::state_        = NiUICoreState();
INiUIDebug*         NiUI::debug_        = nullptr;
NiUIStyle           NiUI::style_        = NiUIStyle();

std::unordered_map<std::string, ButtonData> NiUI::buttonImages_ = std::unordered_map<std::string, ButtonData>();
std::unordered_map<std::string, DivData> NiUI::divData_ = std::unordered_map<std::string, DivData>();

BaseRegionData* NiUI::currentRegion_ = nullptr;



void NiUI::Initialize(const NiVec2& _size, const NiVec2& _leftTop)
{
    state_.valid.isInitialized = true;
    leftTop_ = _leftTop;
    size_ = _size;

    input_.Initialize();

    style_.windowPadding = { 10, 10 };
    style_.color.backGround = { 0.0f, 0.0f, 0.0f, 0.9f };

    return;
}

void NiUI::BeginFrame()
{
    CheckValid_BeginFrame();

    // 入力データ更新
    input_.Update();

    // 入力データのコピー
    CopyInputData();

    // 確認用フラグを立てる
    state_.valid.isBeginFrame = true;

    return;
}

void NiUI::DrawUI()
{
    // 描画処理に必要なデータの確認
    CheckValid_DrawUI();

    // ボタンの確定処理
    PostProcess_Button();

    // コンポーネントの描画データを追加
    ButtonDataEnqueue();
    DivDataEnqueue();

    // 描画前処理
    drawer_->DrawSetting();

    // 描画
    drawer_->Draw();

    // 前フレームのデータとして保存
    SavePreData();

    // データのクリア
    ClearData();

    // 確認用フラグを倒す
    state_.valid.isBeginFrame = false;
}

void NiUI::NiUI_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    input_.WndProcHandler(hWnd, msg, wParam, lParam);
    return;
}



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
    if (currentRegion_ != nullptr)
    {
        posInRegion = _position + currentRegion_->leftTop;
        size = currentRegion_->size;
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

bool NiUI::BeginDiv(const std::string& _id, const NiVec2& _position, const NiVec2& _size, const NiUI_StandardPoint _anchor, const NiUI_StandardPoint _pivot)
{
    state_.valid.nestCount++;

    NiVec2 leftTop = {};

    if (currentRegion_ == nullptr)
    {
        leftTop = ComputeLeftTop(_position, _size, size_, _anchor, _pivot);
    }
    else
    {
        leftTop = ComputeLeftTop(_position, _size, currentRegion_->size, _anchor, _pivot);
    }
    

    divData_[_id].leftTop = leftTop;
    divData_[_id].size = _size;
    divData_[_id].parent = currentRegion_;

    currentRegion_ = dynamic_cast<BaseRegionData*>(&divData_[_id]);

    return true;
}

void NiUI::EndDiv()
{
    if (state_.valid.nestCount == 0)
    {
        throw std::runtime_error("ネスト終了関数の呼び出し過多です。");
    }

    state_.valid.nestCount--;

    currentRegion_ = currentRegion_->parent;
}

bool NiUI::ButtonBehavior(const std::string& _id, bool _isHover, bool _isTrigger, bool _isRelease, bool& _out_held)
{
    if (_isHover)
    {
        state_.componentID.hover = _id;
    }


    if(_isTrigger)
    {
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

void NiUI::DivDataEnqueue()
{
    for (auto& divData : divData_)
    {
        // 描画クラスにデータを追加
        drawer_->EnqueueDrawInfo(&divData.second);
    }
}

void NiUI::PostProcess_Button()
{
    auto& componentID = state_.componentID;
    auto& componentTime = state_.time;

    /// =========
    /// Active
    if (!componentID.active.empty())
    {
        /// アクティブIDが変更されたら
        if (componentID.active != componentID.preActive)
        {
            // アクティブ時間をリセット
            componentTime.active = 0;
        }
        ++componentTime.active;
    }
    else
    {
        componentTime.active = 0;
    }


    /// ========
    /// Hover
    if (!componentID.hover.empty())
    {
        if (componentID.hover != componentID.preHover)
        {
            componentTime.hover = 0;
        }
        ++componentTime.hover;
    }
    else
    {
        componentTime.hover = 0;
    }

    /// ========
    /// Play SE
    if (componentTime.active == 0 && componentID.preActive == componentID.preHover && !componentID.preActive.empty())
    {
        if (io_.audioHandler.buttonConfirm)
        {
            drawer_->PlayAudio(io_.audioHandler.buttonConfirm);
        }
        else
        {
            drawer_->PlayAudio(io_.audioHnd.buttonConfirm);
        }
    }
    if (componentTime.hover == 1)
    {
        if (io_.audioHandler.buttonHover)
        {
            drawer_->PlayAudio(io_.audioHandler.buttonHover);
        }
        else
        {
            drawer_->PlayAudio(io_.audioHnd.buttonHover);
        }
    }
}



void NiUI::CheckValid_BeginFrame()
{
    if(!state_.valid.isInitialized)
    {
        throw std::runtime_error("UIクラスが初期化されていません。");
    }

    if(state_.valid.isBeginFrame)
    {
        throw std::runtime_error("BeginFrameが連続で呼び出されています。");
    }
}

void NiUI::CheckValid_DrawUI()
{
    if(!state_.valid.isInitialized)
    {
        throw std::runtime_error("UIクラスが初期化されていません。");
    }

    if(!state_.valid.isBeginFrame)
    {
        throw std::runtime_error("BeginFrameが呼び出されていません。");
    }

    if(drawer_ == nullptr)
    {
        throw std::runtime_error("Drawerがセットされていません。");
    }

    if (state_.valid.nestCount != 0)
    {
        throw std::runtime_error("ネストが不正です。ネスト開始関数とネスト終了関数の数が正しく呼び出されていますか？");
    }
}

void NiUI::JudgeClickRect(const NiVec2& _leftTop, const NiVec2& _size, bool& _isHover, bool& _isTrigger, bool& _isRelease)
{
    NiVec2 leftTop = _leftTop + leftTop_;

    if(leftTop.x <= input_.GetMousePos().x && input_.GetMousePos().x <= leftTop.x + _size.x &&
        leftTop.y <= input_.GetMousePos().y && input_.GetMousePos().y <= leftTop.y + _size.y)
    {
        _isHover = true;
        if(input_.TriggerLeft())
        {
            _isTrigger = true;
        }
        else
        {
            _isTrigger = false;
        }
    }
    else
    {
        _isHover = false;
        _isTrigger = false;
    }

    if(input_.ReleaseLeft())
    {
        _isRelease = true;
    }
    else
    {
        _isRelease = false;
    }
}

NiVec2 NiUI::ComputeStandardPoint(NiUI_StandardPoint _stdpoint)
{
    NiVec2 result = {};

    switch (_stdpoint)
    {
    case NiUI_StandardPoint::LeftTop:
        result.x = 0.0f;
        result.y = 0.0f;
        break;

    case NiUI_StandardPoint::CenterTop:
        result.x = 0.5f;
        result.y = 0.0f;
        break;

    case NiUI_StandardPoint::RightTop:
        result.x = 1.0f;
        result.y = 0.0f;
        break;

    case NiUI_StandardPoint::LeftCenter:
        result.x = 0.0f;
        result.y = 0.5f;
        break;

    case NiUI_StandardPoint::Center:
        result.x = 0.5f;
        result.y = 0.5f;
        break;

    case NiUI_StandardPoint::RightCenter:
        result.x = 1.0f;
        result.y = 0.5f;
        break;

    case NiUI_StandardPoint::LeftBottom:
        result.x = 0.0f;
        result.y = 1.0f;
        break;

    case NiUI_StandardPoint::CenterBottom:
        result.x = 0.5f;
        result.y = 1.0f;
        break;

    case NiUI_StandardPoint::RightBottom:
        result.x = 1.0f;
        result.y = 1.0f;
        break;

    default:
        break;
    }

    return result;
}

NiVec2 NiUI::ComputeLeftTop(const NiVec2& _position, const NiVec2& _size, const NiVec2& _parentSize, NiUI_StandardPoint _anchor, NiUI_StandardPoint _pivot)
{
    NiVec2 result = {};

    NiVec2 normalAnchor = ComputeStandardPoint(_anchor);
    NiVec2 normalPivot = ComputeStandardPoint(_pivot);

    result = _position + normalAnchor * _parentSize;
    result -= _size * normalPivot;
    return result;
}

void NiUI::ClearData()
{
    buttonImages_.clear();
    state_.componentID.hover = {};
    return;
}

void NiUI::SavePreData()
{
    // 前フレームのホバー中のコンポーネントIDを保存
    state_.componentID.preHover = state_.componentID.hover;
    state_.componentID.preActive = state_.componentID.active;
}

void NiUI::CopyInputData()
{
    io_.input.isLeftPre = io_.input.isLeft;
    io_.input.isRightPre = io_.input.isRight;
    io_.input.isMiddlePre = io_.input.isMiddle;

    io_.input.isLeft = input_.PressLeft();
    io_.input.isRight = input_.PressRight();
    io_.input.isMiddle = input_.PressMiddle();
}


