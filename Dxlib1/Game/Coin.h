#pragma once
#include "../vec2.h"

class Coin
{
public:
	// �R���X�g���N�^
	Coin(int graph);

	// �f�X�g���N�^
	virtual ~Coin(){}

	// ����������
	void Init();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �R�C���̑���
	bool IsEnabled();
private:
	// �R�C���̍��W
	Vec2 pos_;

	// �摜�n���h��
	int imgH_;

	// �R�C���̗����X�s�[�h
	float fallSpeed_;

	// �R�C���̉摜�C���f�b�N�X
	int coinImgIdx_;
};