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

	// コンストラクタ
	Field();
	 
	// デストラクタ
	~Field(){}

	// 更新処理
	void Updata();

	// 描画処理
	void Draw();

	// ゲームクリアチェック判定
	bool IsGameClearCheck();

	// ブロックがあるかどうか
	bool IsBlock(int y, int x );

	// 餌があるかどうか
	bool IsFeed(int y, int x);

	// パワーエサがあるかどうか
	bool IsPowerFeed(int y, int x);

	// 指定の位置にプレイヤーがいる場合
	// 指定の位置にワープさせる
	int  PlayerWorp(int ky, int kx, int x, int y);
private:
	// 表示用タイマー
	int drawTimer_;

	// 表示をさせない用タイマー
	int noDrawTimer_;

	// 表示するかどうか
	bool isDraw_;
};

