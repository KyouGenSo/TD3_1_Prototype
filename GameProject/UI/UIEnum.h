#pragma once

// UI�p�񋓑�
// UI�N���X�Ŏg�p����񋓑̂��`���܂��B
// ���̗񋓑̂�UI�N���X�̓����Ŏg�p����܂��B


/// - - - - - 
/// ���C�A�E�g
/// - - - - - -


// �t���b�N�X�R���e�i�̕���
enum class NiUI_Flex_Direction
{
    Row, // ��
    Column, // �c
};

// �t���b�N�X�R���e�i�̔z�u
enum class NiUI_Flex_JustifyContent
{
    Start, // �J�n
    End, // �I��
    Center, // ����
    SpaceBetween, // �Ԋu���ϓ���
    SpaceAround, // �Ԋu���ϓ���
};

// �t���b�N�X�R���e�i�̃A���C�������g
enum class NiUI_Flex_AlignItems
{
    Start, // �J�n
    End, // �I��
    Center, // ����
};

// �t���b�N�X�A�C�e���𕡐��s�ɂ����Ƃ�
enum class NiUI_Flex_AlignContent
{
    Start, // �J�n
    End, // �I��
    Center, // ����
    SpaceBetween, // �Ԋu���ϓ���
    SpaceAround, // �Ԋu���ϓ���
};


/// - - - - - - - - - - -
/// �R���|�[�l���g�E�X�e�[�g
/// - - - - - - - - - - -

// �{�^���̏��
enum class NiUI_ButtonState
{
    None, // �����Ȃ�
    Hover, // �z�o�[
    Press, // ����
};