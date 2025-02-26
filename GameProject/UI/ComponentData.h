#pragma once

// �R���|�[�l���g�f�[�^
// �R���|�[�l���g�̃f�[�^��ێ�����\���̂ł��B
// ���̍\���̂�UI�N���X�̓����Ŏg�p����܂��B


#include <string> // string
#include "math/NiVec2.h" // NiVec2
#include "UIEnum.h" // enums


// �t���b�N�X�R���e�i�̃f�[�^
struct FlexContainerData
{
    NiVec2 leftTop;
    NiVec2 size;
    NiVec2 padding;
    NiVec2 margin;
    NiUI_Flex_Direction direction;
    NiUI_Flex_JustifyContent justifyContent;
    NiUI_Flex_AlignItems alignItems;
    NiUI_Flex_AlignContent alignContent;
};


// �t���b�N�X�A�C�e���̃f�[�^
struct ButtonImageData
{
    std::string textureName;
    NiVec2 leftTop;
    NiVec2 size;
};