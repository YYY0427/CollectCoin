#pragma once
#include <memory>

// �v���g�^�C�v�錾
class Player;
class GameplayingScene;

/// <summary>
/// �G�̊��N���X
/// </summary>
class EnemyBase
{
public:
	/// <summary>
	/// �G�I�u�W�F�N�g�̍X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �G�I�u�W�F�N�g�̕`��
	/// </summary>
	virtual void Draw() = 0;

protected:	// �q�N���X�ɂ͌����������

	enum DIRECT
	{
		up = 1,
		down,
		left,
		right,
		direct_num
	};

	// �摜�̃n���h��
	int handle_;

	// �\������摜�̃C���f�b�N�X
	int idX_;

	// �����Ă������
	int moveDirection_;

	// �C���f�b�N�X���W
	int indexX_;
	int indexY_;
};
