#pragma once
class Map
{
public:
	Map();
	virtual ~Map();

	//�`��
	void Draw();

private:

	//�}�b�v�̕`��
	void DrawMap();

	//�}�b�v�`�b�v�̐����擾����
	int ChipNumX();
	int ChipNumY();
	int ChipNum();

private:
	//�O���t�B�b�N�f�[�^�̃n���h��
	int handle_;

	//�O���t�B�b�N�̃f�[�^
	int graphWidth_;
	int graphHeight_;

};