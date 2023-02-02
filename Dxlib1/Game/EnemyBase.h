#pragma once
#include <memory>

// �v���g�^�C�v�錾
class Player;

/// <summary>
/// �G�̊��N���X
/// </summary>
class EnemyBase
{
public:
	EnemyBase(std::shared_ptr<Player> player);

	/// <summary>
	/// �G�I�u�W�F�N�g�̍X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �G�I�u�W�F�N�g�̕`��
	/// </summary>
	virtual void Draw() = 0;
protected:	// �q�N���X�ɂ͌����������
	std::shared_ptr<Player> player_= nullptr;
};
