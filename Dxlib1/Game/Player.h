#pragma once
#include <memory>	
#include "../Geometry.h"
#include "../vec2.h"
#include "EnemyBase.h"

class Field;
class InputState;
class EnemyBase;

class Player
{
public:
	// �R���X�g���N�^
	Player(int normalH, int waponH, int deadH, int indexX, int indexY);

	//�f�X�g���N�^
	~Player(){};

	// �|�C���^�̐ݒ�
	void SetEnemy(std::shared_ptr<EnemyBase>enemy, int i) { pEnemy_[i] = enemy; }
	void SetField(std::shared_ptr<Field>field) { pField_ = field; }

	void Init();

	// ����
	void Update(const InputState& input);

	// �G�����񂾏ꍇ�̏��� 
	void DeadUpdate();

	// �`��
	void Draw();

	// �����蔻��
	bool Colision(int direction);

	// �p���[�G�T���擾�����ꍇ�̑��x�v�Z
	void SpeedCalculation();

	// �C���f�b�N�X���W�����W�ɕϊ�
	void PosCalculation();

	// �v���C���[�̍��W�̎擾
	Vec2 GetSize() const { return deathImgSize_; }
	Vec2 GetPos() const { return pos_; }

	int GetIndexX() const { return indexX_; }

	int GetIndexY() const { return indexY_; }

	// �p���[�G�T���擾�������ǂ���
	bool GetPowerFeed() const { return isPowerFeed_; }

	bool GetDead() const { return isDead_; }

	// ���S�ݒ�
	void SetDead(bool isDead) { isDead_ = isDead; }

	// ���ݐݒ�
	void SetEnabled(bool isEnabled) { isEnabled_ = isEnabled; }

	// ���S���̃A�j���[�V�����̏I�����邩�ǂ������擾
	bool GetAnimeEnd() const { return isAnimeEnd_; }

	// ����
	enum Direct
	{
		// 1����J�E���g
		up = 1,
		down,
		left,
		right,
		directNum
	};

private:

	// �|�C���^
	std::shared_ptr<Field> pField_;	
	std::shared_ptr<EnemyBase> pEnemy_[EnemyBase::enemy_num];

	Vec2 deathImgSize_;
	
	// �v���C���[�̍��W
	Vec2 pos_;

	//�v���C���[�̃C���f�b�N�X���W
	int indexX_;
	int indexY_;

	// �v���C���[�̉��̍��W
	int kX_;
	int kY_;

	bool isTurnFlag_;
	bool isFlash_;

	int timer_;

	//�v���C���[�̉摜
	int handle_;
	int normalH_;
	int waponH_;
	int deathH_;
	
	// �ړ��C���^�[�o���p�^�C�}�[
	int moveTimer_;

	// �\������摜�̃C���f�b�N�X
	int imgIdX_;
	int deadImgIdx_;

	// �摜�̕����̐؂�ւ�
	float angle_;

	// �v���C���[���ǂ̕����ɐi��ł��邩
	int moveDirection_;

	// �v���C���[���ǂ̕����ɐi�݂�����
	int wantMoveDirection_;

	// �擾�����G�T�̐�
	int feedGetNum_;

	// �v���C���[�̈ړ����x
	int moveInterval_;

	// �p���[�G�T���擾�������ǂ���
	bool isPowerFeed_;

	// �p���[�G�T���擾������ԂŃp���[�G�T���擾������
	bool isPowerFeed2_;
	
	// �p���[�G�T���擾������Ԃłǂ̂��炢�o������
	int powerFeedTimer_;

	// �p���[�G�T���擾�����ꍇ�̃X�s�[�h
	float speed2_;

	// �v���C���[���G�Ɠ���������
	bool isDead_;

	// ���S���A�j���[�V�������I�����邩
	bool isAnimeEnd_;

	float speed_;

	bool isEnabled_;

	// ����̃u���b�N�ɐN���\��
	bool isIntrusion_;

	int imgY_;
};