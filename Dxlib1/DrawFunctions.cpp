#include <DxLib.h>
#include <cassert>
#include "DrawFunctions.h"

namespace my
{
	/// <summary>
	/// �摜�����[�h����
	/// </summary>
	/// <param name="path">�p�X������</param>
	/// <returns>�摜�̃n���h��</returns>
	int MyLoadGraph(const TCHAR* path)
	{
		int handle = LoadGraph(path);
		assert(handle >= 0);
		return handle;
	}

	/// <summary>
	/// �T�E���h�����[�h����
	/// </summary>
	/// <param name="path">�p�X������</param>
	/// <returns>�T�E���h�̃n���h��</returns>
	int MyLoadSound(const TCHAR* path)
	{
		int handle = LoadSoundMem(path);
		assert(handle >= 0);
		return handle;
	}
}//end of namespace my


