#pragma once
#include "../Geometry.h"

class Enemy;
class Field;
class InputState;

class Player
{
public:
	// �R���X�g���N�^
	Player();

	//�f�X�g���N�^
	~Player();

	// ����
	void Update(const InputState& input);

	// �`��
	void Draw();

	// �A�j���[�V����
	void Animation();
private:
	Enemy* pEnemy_ = nullptr;
	Field* pField_ = nullptr;

	// ���@�̌��݂̍��W
	Position2 pos_;	

	//�v���C���[�̍��W
	int indexX_;
	int indexY_;

	// �v���C���[�̉��̍��W
	int kX_;
	int kY_;

	//�v���C���[���E�������͉��Ɉړ����Ă��邩
	bool isMoveDownOrRight;

	//�v���C���[�̉摜
	int handle_;

	// �ړ��C���^�[�o���p�^�C�}�[
	int moveTimer_;

	// �摜�̐؂�ւ��p�^�C�}�[
	int animeTimer_;

	// �\������摜�̃C���f�b�N�X
	int imgIdx = 0;	

	// �摜�̕����̐؂�ւ�
	float angle_;

	//�v���C���[���ǂ̕����Ɉړ����Ă��邩
	int moveDirection;
};