#pragma once
#include "EnemyBase.h"
#include <memory>

class Field;

/// <summary>
/// �S���^�C�v�G
/// </summary>
class ChasingEnemy : public EnemyBase
{
public:
	ChasingEnemy();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw()override;

	// �����蔻��
	bool Colision(int direction);

	// �p���[�G�T���擾�����ꍇ�̑��x�v�Z
	void SpeedCalculation();

	// �C���f�b�N�X���W�����W�ɕϊ�
	void PosCalculation();

	/// <summary>
	/// �����Ă�������ɂ���Ēl��Ԃ�
	/// </summary>
	/// <returns>�摜��y���W</returns>
	virtual int DirectReturnNum();
private:
	std::shared_ptr<Field> pField_;

	// �v���C���[�̉��̍��W
	int kX_;
	int kY_;

	// �ړ��C���^�[�o���p�^�C�}�[
	int moveTimer_;

	// �v���C���[���ǂ̕����ɐi�݂�����
	int wantMoveDirection_;

	// �v���C���[�̈ړ����x
	int moveSpeed_;

	// �p���[�G�T���擾�����ꍇ�̃X�s�[�h
	float powerFeedSpeed_;

	// �p���[�G�T���擾�������ǂ���
	bool isPowerFeed_;

	// �p���[�G�T���擾������Ԃłǂ̂��炢�o������
	int powerFeedTimer_;

	int a;
};

