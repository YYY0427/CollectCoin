#pragma once

class Field
{
public:
	// マップの横幅
	static constexpr int MAP_WIDTH = 19;
	// マップの縦幅
	static constexpr int MAP_HEIGHT = 22;
	// ブロックの大きさ
	static constexpr int BLOCK_SIZE = 32;

	// コンストラクタ
	Field();
	 
	// デストラクタ
	~Field(){}

	// 更新処理
	void Updata();

	// 描画処理
	void Draw();
	void PowerFeedFlash(int y, int x);

	// パワーエサの点滅
	void Flash();

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
	// パワーエサの点滅
	int blendCount_;
	bool blendLimitMax_;

	// 表示するかどうか
	bool isDraw_;

	// パワーエサのハンドル
	int handle_;
};