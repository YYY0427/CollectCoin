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

	/// <summary>
	/// �T�E���h�����[�h����
	/// </summary>
	/// <param name="path">�p�X������</param>
	/// <returns>�T�E���h�̃n���h��</returns>
	int MyLoadSound(const TCHAR* path);
}
