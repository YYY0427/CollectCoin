#pragma once

namespace Game
{
	//�E�B���h�E���[�h�ݒ�
#ifdef _DEBUG
	constexpr bool kWindowMode = true;		// �f�o�b�N�̔ŏꍇ
#else
	constexpr bool kWindowMode = false;		// �����[�X�ł̏ꍇ
#endif

	// �Q�[���E�B���h�E��
	const char* const kTitleText = "�f�`�l�d";

	//�E�B���h�E�T�C�Y
	constexpr int SCREEN_WIDTH = 1600;
	constexpr int SCREEN_HEIGHT = 900;

	//�J���[���[�h
	constexpr int kColorDepth = 32;   //32 or 16

};