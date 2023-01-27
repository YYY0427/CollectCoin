#pragma once
#include <tchar.h>

//�����p���C�u�����p���O���
namespace my 
{
	/// <summary>
	/// �摜�����[�h����
	/// </summary>
	/// <param name="path">�p�X������</param>
	/// <returns>�摜�̃n���h��</returns>
	int MyLoadGraph(const TCHAR* path);
}

namespace Game
{
	// �E�C���h�E���[�h�ݒ�
	constexpr bool kWindowMode = true;
	// �E�C���h�E��
	const char* const kTitleText = "�p�b�N�}��";
	// �E�C���h�E�T�C�Y
	constexpr int kScreenWidth = 640;
	constexpr int kScreenHeight = 720;
	// �J���[���[�h
	constexpr int kColorDepth = 32;		// 32 or 16
};
