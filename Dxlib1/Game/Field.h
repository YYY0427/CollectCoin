#pragma once

class Field
{
public:
	// �}�b�v�̉���
	static constexpr int MAP_WIDTH = 19;
	// �}�b�v�̏c��
	static constexpr int MAP_HEIGHT = 22;
	// �u���b�N�̑傫��
	static constexpr int BLOCK_SIZE = 32;

	// �R���X�g���N�^
	Field();
	 
	// �f�X�g���N�^
	~Field(){}

	// �X�V����
	void Updata();

	// �`�揈��
	void Draw();
	void PowerFeedFlash(int y, int x);

	// �p���[�G�T�̓_��
	void Flash();

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
	// �p���[�G�T�̓_��
	int blendCount_;
	bool blendLimitMax_;

	// �\�����邩�ǂ���
	bool isDraw_;

	// �p���[�G�T�̃n���h��
	int handle_;
};