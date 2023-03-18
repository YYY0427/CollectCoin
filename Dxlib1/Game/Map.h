#pragma once
#include "Field.h"
#include <memory>
#include <vector>

class Map
{
public:
	Map(int handle, int stage);
	virtual ~Map(){};

	void SetField(std::shared_ptr<Field>field) { pField_ = field; }

	void StageCheck(int stage);
	void StageCheck2(int stage);

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
	
	std::shared_ptr<Field> pField_;

	std::vector<std::vector<int>> mapData_;

	int disPlayPosX_;
	int disPlayPosY_;

	int mapHeight_;
	int mapWidth_;

	int stage_;

	//�O���t�B�b�N�f�[�^�̃n���h��
	int handle_;

	//�O���t�B�b�N�̃f�[�^
	int graphWidth_;
	int graphHeight_;
};