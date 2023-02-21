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
	virtual ~EnemyBase(){}

	// �|�C���^�̐ݒ�
	void SetPlayer(std::shared_ptr<Player>player) { pPlayer_ = player; }
	void SetField(std::shared_ptr<Field>field) { pField_ = field; }

	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();


	// ���ݐi��ł�������ւ̓����蔻��
	bool Colision(int direction);

	// �u���b�N���Ȃ�������������
	int NoBlockDirect(int indexX, int indexY);

	// �ړ��X�s�[�h�̕ύX
	void SpeedChange();

	// �C���f�b�N�X���W�����W�ɕϊ�
	void PosCalculation();

	// �G�����񂾂Ƃ��̏�����
	void SetDeadInit();

	virtual void SetInit() = 0;

	// �G�̍��W�̎擾
	Vec2 GetSize()const { return size_; }
	Vec2 GetPos() const { return pos_; }

	bool GetIzike()const { return isIzike_; }
	bool GetTracking()const { return isTracking_; }
	int  GetIndexY()const { return indexY_; }
	int  GetIndexX()const { return indexX_; }
	bool GetMove() const { return isMove_; }

	// �G�̕\���������������Ȃ������Z�b�g
	void SetEnabled(bool isEnabled) { isEnabled_ = isEnabled; }

	// �G�����񂾎���łȂ������Z�b�g
	void SetDead(bool isDead) { isDead_ = isDead; }

	// �G���C�W�P��Ԃ����Z�b�g
	void SetIzike(bool isIzike) { isIzike_ = isIzike; }

	// �G�𔒂����点�邩
	void SetFlash(bool isFlash) { isFlash_ = isFlash; }

	// �꒣�胂�[�h�ƒǐՃ��[�h�̐؂�ւ�
	void ModeSwitch();
	
	void MoveSwitch(int startInterval, int deadInterval);

	/// <summary>
	/// �����Ă�������ɂ���Ēl��Ԃ�
	/// </summary>
	/// <returns>�摜��y���W</returns>
	virtual int DirectReturnNum();

	// ����
	enum 
	{
		up = 1,
		down,
		left,
		right,
		direct_num
	};

	// �G�̎��
	enum 
	{
		blinky,
		inky, 
		cryde,
		pinky,
		enemy_num
	};
protected:
	std::shared_ptr<Field> pField_ = nullptr;
	std::shared_ptr<Player> pPlayer_ = nullptr;

	// �摜�T�C�Y
	Vec2 size_;

	// ���W
	Vec2 pos_;

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

	// �ǐՃ��[�h���ǂ���
	bool isTracking_;

	int trackingTimer_;

	// �G�𓮂�����
	bool isMove_;

	// ����̃u���b�N�ɐN���\��
	bool isIntrusion_;

	int moveTimer2_;
};
