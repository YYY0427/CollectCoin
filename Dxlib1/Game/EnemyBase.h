#pragma once
#include <memory>
#include "../Geometry.h"
#include "../vec2.h"

// �v���g�^�C�v�錾
class Player;
class Field;

/// <summary>
/// �G�̊��N���X
/// </summary>
class EnemyBase
{
public:
	EnemyBase();

	// �|�C���^�̐ݒ�
	void SetPlayer(std::shared_ptr<Player>player) { pPlayer_ = player; }
	void SetField(std::shared_ptr<Field>field) { pField_ = field; }

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	// �����蔻��
	bool Colision(int direction);

	// �p���[�G�T���擾�����ꍇ�̑��x�v�Z
	void SpeedCalculation();

	// �C���f�b�N�X���W�����W�ɕϊ�
	void PosCalculation();

	// �G�����񂾂Ƃ��̏�����
	void SetInit();

	// �G�̍��W�̎擾
	Vec2 GetSize()const { return size_; }
	Vec2 GetPos() const { return pos_; }

	bool GetIzike()const { return isIzike_; }

	// �G�̕\���������������Ȃ������Z�b�g
	void SetEnabled(bool isEnabled) { isEnabled_ = isEnabled; }

	// �G�����񂾎���łȂ������Z�b�g
	void SetDead(bool isDead) { isDead_ = isDead; }

	// �G���C�W�P��Ԃ����Z�b�g
	void SetIzike(bool isIzike) { isIzike_ = isIzike; }

	void SetFlash(bool isFlash) { isFlash_ = isFlash; }

	/// <summary>
	/// �����Ă�������ɂ���Ēl��Ԃ�
	/// </summary>
	/// <returns>�摜��y���W</returns>
	virtual int DirectReturnNum();
protected:
	std::shared_ptr<Field> pField_ = nullptr;
	std::shared_ptr<Player> pPlayer_ = nullptr;

	// �摜�T�C�Y
	Vec2 size_;

	// ���W
	Vec2 pos_;

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
	int izikeHandle_;

	// �\������摜�̃C���f�b�N�X
	int idX_;
	int idY_;

	// �����Ă������
	int moveDirection_;

	// �C���f�b�N�X���W
	int indexX_;
	int indexY_;

	// ���̍��W
	int kX_;
	int kY_;

	// �ړ��C���^�[�o���p�^�C�}�[
	int moveTimer_;

	// �v���C���[���ǂ̕����ɐi�݂�����
	int wantMoveDirection_;

	// �v���C���[�̈ړ����x
	int moveInterval_;

	// �p���[�G�T���擾�����ꍇ�̃X�s�[�h
	float speed_;

	// �p���[�G�T���擾�������ǂ���
	bool isPowerFeed_;

	// �p���[�G�T���擾������Ԃłǂ̂��炢�o������
	int powerFeedTimer_;

	// �\�����邩
	bool isEnabled_;

	// �t���b�V�������Ă���ꍇ�̉摜�̃C���f�b�N�X
	int flashingImgY_;

	// ���񂾂�
	bool isDead_;

	// �C�W�P��Ԃ�
	bool isIzike_;

	// �t���b�V�������邩
	bool isFlash_;
};
