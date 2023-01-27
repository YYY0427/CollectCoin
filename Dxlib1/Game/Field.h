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

	Field();
	~Field(){}

	void Updata();
	void Draw();

	// �u���b�N�����邩�ǂ���
	bool IsBlock(int y, int x );

	// �a�����邩�ǂ���
	bool IsFeed(int y, int x);

	// �p���[�G�T�����邩�ǂ���
	bool IsPowerFeed(int y, int x);
private:
	// �\���p�^�C�}�[
	int drawTimer_;
	// �\���������Ȃ��p�^�C�}�[
	int noDrawTimer_;

	// �\�����邩�ǂ���
	bool isDraw_;
};

