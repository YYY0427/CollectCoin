#pragma once

class Field
{
public:

	// �}�b�v�̉���
	static constexpr int kMapWidth = 19;
	// �}�b�v�̏c��
	static constexpr int kMapHeight = 22;
	// �u���b�N�̑傫��
	static constexpr int kBlockSize = 32;

	// �R���X�g���N�^
	Field();
	 
	// �f�X�g���N�^
	~Field(){}

	// �X�V����
	void Updata();

	// �`�揈��
	void Draw();

	// �Q�[���N���A�`�F�b�N����
	bool IsGameClearCheck();

	// �u���b�N�����邩�ǂ���
	bool IsBlock(int y, int x );

	// �a�����邩�ǂ���
	bool IsFeed(int y, int x);

	// �p���[�G�T�����邩�ǂ���
	bool IsPowerFeed(int y, int x);

	// �w��̈ʒu�Ƀv���C���[������ꍇ
	// �w��̈ʒu�Ƀ��[�v������
	int  PlayerWorp(int ky, int kx, int x, int y);
private:
	// �\���p�^�C�}�[
	int drawTimer_;

	// �\���������Ȃ��p�^�C�}�[
	int noDrawTimer_;

	// �\�����邩�ǂ���
	bool isDraw_;
};

