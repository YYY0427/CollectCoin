#pragma once
//�u�X�}�[�g�|�C���^�v���g�����߂�include
#include <memory>	
#include "../Geometry.h"

class Field;
class InputState;

class Player
{
public:
	// �R���X�g���N�^
	Player();

	//�f�X�g���N�^
	~Player(){};

	// ����
	void Update(const InputState& input);

	// �`��
	void Draw();

	// �����蔻��
	bool Colision(int direction);

	// �p���[�G�T���擾�����ꍇ�̑��x�v�Z
	void SpeedCalculation();

	// �C���f�b�N�X���W�����W�ɕϊ�
	void PosCalculation();

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

	// ���@�̌��݂̍��W
	Position2 pos_;	

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

	// �ړ��C���^�[�o���p�^�C�}�[
	int moveTimer_;

	// �\������摜�̃C���f�b�N�X
	int imgIdX_;	

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

	float powerFeedSpeed_;
};