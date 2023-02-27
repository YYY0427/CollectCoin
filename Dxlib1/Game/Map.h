#pragma once
class Map
{
public:
	Map();
	virtual ~Map();

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
	//グラフィックデータのハンドル
	int handle_;

	//グラフィックのデータ
	int graphWidth_;
	int graphHeight_;

};