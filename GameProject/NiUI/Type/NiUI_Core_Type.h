#pragma once

#include <string> // std::string


// Included in NiUICoreState
struct NiUIComponentID
{
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
struct NiUIValidFlag
{
    bool isInitialized;
    bool isBeginFrame;
};

// Included in NiUIIO
struct NiUIInputData
{
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


struct NiUIIO
{
    NiUIInputData input;
    NiUIAudioHandle audioHnd;
};


struct NiUICoreState
{
    NiUIValidFlag validFlag;
    NiUIComponentID componentID;
    NiUITime time;
};