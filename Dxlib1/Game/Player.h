#pragma once
//�u�X�}�[�g�|�C���^�v���g�����߂�include
#include <memory>	
#include "../Geometry.h"
#include "../vec2.h"

class Field;
class ChasingEnemy;
class InputState;

class Player
{
public:
	// �R���X�g���N�^
	Player(std::shared_ptr<Field>field, std::shared_ptr<ChasingEnemy>chasingEnemy);

	//�f�X�g���N�^
	~Player(){};

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
	Vec2 GetSize() const { return size_; }
	Vec2 GetPos() const { return pos_; }

	// �p���[�G�T���擾�������ǂ���
	bool GetPowerFeed() const { return isPowerFeed_; }

	bool GetEnemyFlashing()const { return isEnemyFlashing_; }

	// ���S�ݒ�
	void SetDead(bool isDead) { isDead_ = isDead; }

	// ���S���̃A�j���[�V�����C���f�b�N�X�̎擾
	bool GetEnd() const { return isAnimeEnd_; }

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

	//�t�B�[���h
	std::shared_ptr<Field> pField_;
	
	std::shared_ptr<ChasingEnemy> pChasingEnemy_;

	Vec2 size_;

	Vec2 pos_;

	// �v���C���[�̍��W
	int posX_;
	int posY_;

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
	int moveSpeed_;

	// �p���[�G�T���擾�������ǂ���
	bool isPowerFeed_;
	
	// �p���[�G�T���擾������Ԃłǂ̂��炢�o������
	int powerFeedTimer_;

	// �p���[�G�T���擾�����ꍇ�̃X�s�[�h
	float powerFeedSpeed_;

	// �v���C���[���G�Ɠ���������
	bool isDead_;

	// 
	bool isAnimeEnd_;

	bool isEnemyFlashing_;
};