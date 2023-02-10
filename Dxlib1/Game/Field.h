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

	// 表示場所
	static constexpr int DISPLAY_POS_X = 400;
	static constexpr int DISPLAY_POS_Y = 100;

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
	int  Worp(int ky, int kx, int x, int y);

	// playerまでの最短経路を探すための準備として、コピーマップの初期化と目的地(player)の場所に10(目印)を設定
	void MoveDataSet(int playerY, int playerX);

	// 現在地からプレイヤーまでの最短経路を探す関数
	void Search(int y, int x, int pos);
private:

	int mapData2[MAP_HEIGHT][MAP_WIDTH];

	// パワーエサの点滅
	int blendCount_;
	bool blendLimitMax_;

	// 表示するかどうか
	bool isDraw_;

	// パワーエサのハンドル
	int handle_;
};