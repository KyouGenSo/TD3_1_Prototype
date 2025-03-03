#include "NiUI.h"

#include <stdexcept> // runtime_error


NiUI_Input          NiUI::input_        = NiUI_Input();
NiVec2              NiUI::leftTop_      = { 0, 0 };
NiVec2              NiUI::size_         = { 0, 0 };
IDrawer*            NiUI::drawer_       = nullptr;

NiUIIO              NiUI::io_           = NiUIIO();
NiUICoreState       NiUI::state_        = NiUICoreState();
INiUIDebug*         NiUI::debug_        = nullptr;

std::unordered_map<std::string, ButtonData> NiUI::buttonImages_ = std::unordered_map<std::string, ButtonData>();



void NiUI::Initialize(const NiVec2& _size, const NiVec2& _leftTop)
{
    state_.validFlag.isInitialized = true;
    leftTop_ = _leftTop;
    size_ = _size;

    input_.Initialize();

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
    state_.validFlag.isBeginFrame = true;

    return;
}

void NiUI::DrawUI()
{
    // 描画処理に必要なデータの確認
    CheckValid_DrawUI();

    // ボタンの確定処理
    PostProcess_Button();

    // ボタンの描画データを追加
    ButtonDataEnqueue();

    // 描画前処理
    drawer_->DrawSetting();

    // 描画
    drawer_->Draw();

    // 前フレームのデータとして保存
    SavePreData();

    // データのクリア
    ClearData();

    // 確認用フラグを倒す
    state_.validFlag.isBeginFrame = false;
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

    NiVec2 normalAnchor = ComputeStandardPoint(_anchor);
    NiVec2 normalPivot = ComputeStandardPoint(_pivot);

    NiVec2 position = _position + normalAnchor * size_;
    position -= _size * normalPivot;

    // 当たり判定
    JudgeClickRect(position, _size, isHover, isTrigger, isRelease);

    // ボタンの挙動
    bool onButton = ButtonBehavior(_id, isHover, isTrigger, isRelease, isHeld);

    /// ボタンのデータを更新
    buttonImage.id = _id;
    buttonImage.textureName = _textureName;
    buttonImage.leftTop = position;
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
        drawer_->PlayAudio(io_.audioHnd.buttonConfirm);
    }
    if (componentTime.hover == 1)
    {
        drawer_->PlayAudio(io_.audioHnd.buttonHover);
    }
}



void NiUI::CheckValid_BeginFrame()
{
    if(!state_.validFlag.isInitialized)
    {
        throw std::runtime_error("UIクラスが初期化されていません。");
    }

    if(state_.validFlag.isBeginFrame)
    {
        throw std::runtime_error("BeginFrameが連続で呼び出されています。");
    }
}

void NiUI::CheckValid_DrawUI()
{
    if(!state_.validFlag.isInitialized)
    {
        throw std::runtime_error("UIクラスが初期化されていません。");
    }

    if(!state_.validFlag.isBeginFrame)
    {
        throw std::runtime_error("BeginFrameが呼び出されていません。");
    }

    if(drawer_ == nullptr)
    {
        throw std::runtime_error("Drawerがセットされていません。");
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


