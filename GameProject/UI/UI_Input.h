#pragma once

#include "math/NiVec2.h" // NiVec2
#include <minwindef.h>
#include <windef.h>

/// �}�E�X�f�[�^
struct MouseData
{
    NiVec2 pos;
    bool isLeft;
    bool isLeftPre;
    bool isRight;
    bool isRightPre;
    bool isMiddle;
    bool isMiddlePre;
};


/// UI���̓N���X
class UI_Input
{
public: /// �R���X�g���N�^�ƃf�X�g���N�^
    UI_Input() = default;
    ~UI_Input() = default;


public: /// ���

    // ������
    void Initialize();

    // ���̓f�[�^�X�V
    // �f�[�^���擾����O�ɌĂяo���Ă��������B
    void Update();

    // �E�B���h�E�v���V�[�W���n���h��
    void WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


public: // ���̓f�[�^�擾

    // ���N���b�N�v���X
    bool PressLeft() const { return mouseData_.isLeft; }
    // ���N���b�N�g���K�[
    bool TriggerLeft() const { return mouseData_.isLeft && !mouseData_.isLeftPre; }

    // �E�N���b�N�v���X
    bool PressRight() const { return mouseData_.isRight; }
    // �E�N���b�N�g���K�[
    bool TriggerRight() const { return mouseData_.isRight && !mouseData_.isRightPre; }

    // ���N���b�N�v���X
    bool PressMiddle() const { return mouseData_.isMiddle; }
    // ���N���b�N�g���K�[
    bool TriggerMiddle() const { return mouseData_.isMiddle && !mouseData_.isMiddlePre; }


private:
    // �}�E�X�f�[�^
    MouseData mouseData_;
};