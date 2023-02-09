#pragma once
//�u�X�}�[�g�|�C���^�v���g�����߂�include
#include <memory>	
#include "../Geometry.h"
#include "../vec2.h"

class Field;
class InputState;
class EnemyBase;

class Player
{
public:
	// �R���X�g���N�^
	Player();

	//�f�X�g���N�^
	~Player(){};

	// �|�C���^�̐ݒ�
	void SetEnemy(std::shared_ptr<EnemyBase>enemy, int i) { pEnemy_[i] = enemy; }
	void SetField(std::shared_ptr<Field>field) { pField_ = field; }

	// ����
	void Update(const InputState& input);

	// �`��
	void Draw();

	// 
	void DeadUpdate();

	// �����蔻��
	bool Colision(int direction);

	// �p���[�G�T���擾�����ꍇ�̑��x�v�Z
	void SpeedCalculation();

	// �C���f�b�N�X���W�����W�ɕϊ�
	void PosCalculation();

	// �v���C���[�̍��W�̎擾
	Vec2 GetSize() const { return deathImgSize_; }
	Vec2 GetPos() const { return pos_; }

	// �p���[�G�T���擾�������ǂ���
	bool GetPowerFeed() const { return isPowerFeed_; }

	// ���S�ݒ�
	void SetDead(bool isDead) { isDead_ = isDead; }

	// ���S���̃A�j���[�V�����̏I�����邩�ǂ������擾
	bool GetAnimeEnd() const { return isAnimeEnd_; }

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
	std::shared_ptr<EnemyBase> pEnemy_[4];

	Vec2 deathImgSize_;
	
	// �v���C���[�̍��W
	Vec2 pos_;

	//�v���C���[�̃C���f�b�N�X���W
	int indexX_;
	int indexY_;

	// �v���C���[�̉��̍��W
	int kX_;
	int kY_;

	//�v���C���[�̉摜
	int handle_;
	int deathHandle_;

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
	
	// �p���[�G�T���擾������Ԃłǂ̂��炢�o������
	int powerFeedTimer_;

	// �p���[�G�T���擾�����ꍇ�̃X�s�[�h
	float speed2_;

	// �v���C���[���G�Ɠ���������
	bool isDead_;

	// ���S���A�j���[�V�������I�����邩
	bool isAnimeEnd_;

	float speed_;
};