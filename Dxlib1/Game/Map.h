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

	//描画
	void Draw();

private:
	//マップの描画
	void DrawMap();

	//マップチップの数を取得する
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

	//グラフィックデータのハンドル
	int handle_;

	//グラフィックのデータ
	int graphWidth_;
	int graphHeight_;
};