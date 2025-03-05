#pragma once

#include <string> // std::string

#include "../Math/NiVec2.h" // NiVec2
#include "../Math/NiVec4.h" // NiVec4
#include "NiUI_ComponentData.h"


// Included in NiUICoreState
struct NiUIComponentID
{
    std::string type;
    std::string active;
    std::string hover;
    std::string preHover;
    std::string preActive;
};

// Included in NiUICoreState
struct NiUITime
{
    uint32_t hover;
    uint32_t active;
};

// Included in NiUICoreState
struct NiUIValidation
{
    bool isInitialized;
    bool isBeginFrame;
    uint32_t nestCount;
};

struct NiUIBuffer
{
    BaseRegionData* currentRegion;
};

// Included in NiUIIO
struct NiUIInputData
{
    NiVec2 cursorPos;
    NiVec2 triggeredPos;
    NiVec2 differencePos;
    bool isLeft;
    bool isLeftPre;
    bool isRight;
    bool isRightPre;
    bool isMiddle;
    bool isMiddlePre;
};

// Included in NiUIIO
struct NiUIAudioHandle
{
    int32_t buttonHover = -1;
    int32_t buttonConfirm = -1;
};

// Included in NiUIIO
struct NiUIAudioHandler
{
    void* buttonHover;
    void* buttonConfirm;
};

// Included in NiUIStyle
struct NiUIColor
{
    NiVec4 backGround;
};

struct NiUIStyle
{
    NiVec2 windowPadding;
    NiUIColor color;
};


struct NiUIIO
{
    NiUIInputData input;
    NiUIAudioHandle audioHnd;
    NiUIAudioHandler audioHandler;
};


struct NiUICoreState
{
    NiUIValidation valid;
    NiUIComponentID componentID;
    NiUITime time;
    NiUIBuffer buffer;
};