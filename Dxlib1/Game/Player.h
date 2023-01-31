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

	// �A�j���[�V����
	void Animation();
private:
	//�t�B�[���h
	std::shared_ptr<Field> pField_;

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
	int imgIdx_;	

	// �摜�̕����̐؂�ւ�
	float angle_;

	//�v���C���[���ǂ̕����Ɉړ����Ă��邩
	int moveDirection_;

	// �擾�����G�T�̐�
	int feedGetNum_;

	// �v���C���[�̈ړ����x
	int moveSpeed_;

	// �p���[�G�T���擾�������ǂ���
	bool isPowerFeed_;
	
	// �p���[�G�T���擾������Ԃłǂ̂��炢�o������
	int powerFeedTimer_;
};