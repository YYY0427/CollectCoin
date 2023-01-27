#pragma once

class Field
{
public:

	// マップの横幅
	static constexpr int kMapWidth = 19;
	// マップの縦幅
	static constexpr int kMapHeight = 22;
	// ブロックの大きさ
	static constexpr int kBlockSize = 32;

	Field();
	~Field(){}

	void Updata();
	void Draw();

	// ブロックがあるかどうか
	bool IsBlock(int y, int x );

	// 餌があるかどうか
	bool IsFeed(int y, int x);

	// パワーエサがあるかどうか
	bool IsPowerFeed(int y, int x);
private:
	// 表示用タイマー
	int drawTimer_;
	// 表示をさせない用タイマー
	int noDrawTimer_;

	// 表示するかどうか
	bool isDraw_;
};

