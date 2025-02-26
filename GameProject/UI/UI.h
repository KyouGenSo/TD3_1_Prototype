#pragma once

#include "ComponentData.h" // ComponentData
#include "math/NiVec2.h" // NiVec2
#include <unordered_map> // unordered_map
#include <string> // string

/// UI�N���X
class UI
{
public: /// �R���X�g���N�^�ƃf�X�g���N�^
    UI() = default;
    ~UI() = default;


public: /// ���

    // ���[�g�E�B���h�E�̏�����
    // �N���C�A���g�̈�̍�����W�ƃT�C�Y���w�肵�Ă��������B
    static void Initialize(const NiVec2& _size, const NiVec2& _leftTop = {0, 0});


    // �t���[�����Ƃ̏���������
    // UI�R���|�[�l���g��ǉ�����O�ɌĂяo���Ă��������B
    static void BeginFrame();


    // UI�̍X�V�y�ѕ`��
    // ���̊֐����Ăяo������ABeginFrame()���Ăяo���܂�UI�R���|�[�l���g�̒ǉ��͔��f����܂���B
    static void DrawUI();


private:

    // �m�F�p�t���O
    static bool isInitialized_;
    static bool isBeginFrame_;


    // �E�B���h�E�̃T�C�Y
    static NiVec2 leftTop_;
    static NiVec2 size_;


    // �R���|�[�l���g�̃��X�g
    static std::unordered_map<std::string, ButtonImageData> buttonImages_;


private:
    static void CheckValid_BeginFrame();
    static void CheckValid_DrawUI();
};