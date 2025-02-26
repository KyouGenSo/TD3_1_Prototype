#include "UI.h"

#include <stdexcept> // runtime_error



bool UI::isInitialized_ = false;
bool UI::isBeginFrame_ = false;
NiVec2 UI::leftTop_ = { 0, 0 };
NiVec2 UI::size_ = { 0, 0 };
std::unordered_map<std::string, ButtonImageData> UI::buttonImages_ = std::unordered_map<std::string, ButtonImageData>();

void UI::Initialize(const NiVec2& _size, const NiVec2& _leftTop)
{
    isInitialized_ = true;
    leftTop_ = _leftTop;
    size_ = _size;

    return;
}


void UI::BeginFrame()
{
    CheckValid_BeginFrame();



    isBeginFrame_ = true;

    return;
}

void UI::DrawUI()
{
    CheckValid_DrawUI();

    isBeginFrame_ = false;
}

void UI::NiUI_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

}




void UI::CheckValid_BeginFrame()
{
    if(!isInitialized_)
    {
        throw std::runtime_error("UI�N���X������������Ă��܂���B");
    }

    if(isBeginFrame_)
    {
        throw std::runtime_error("BeginFrame���A���ŌĂяo����Ă��܂��B");
    }
}

void UI::CheckValid_DrawUI()
{
    if(!isInitialized_)
    {
        throw std::runtime_error("UI�N���X������������Ă��܂���B");
    }

    if(!isBeginFrame_)
    {
        throw std::runtime_error("BeginFrame���Ăяo����Ă��܂���B");
    }
}
