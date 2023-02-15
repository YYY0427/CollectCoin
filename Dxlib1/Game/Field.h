#pragma once
#include <memory>

class Player;
class EnemyBase;

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

	void SetEnemy(std::shared_ptr<EnemyBase>enemy, int i) { pEnemy_[i] = enemy; }
	void SetPlayer(std::shared_ptr<Player>player) { pPlayer_ = player; }

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

	// フィールドにプレイヤーからの距離を求める関数
	void Search(int y, int x, int pos);

	// mapData2を参照してプレイヤーからの数字が小さい方向の値を返す 
	// プレイヤーがパワーエサを取得していたら大きい方向の値を返す
	int BlinkyMove(int enemyIndexY, int enemyIndexX);

	int PinkyMove(int enemyIndexY, int enemyIndexX);

	int InkyMove(int enemyIndexY, int enemyIndexX);

	int CrydeMove(int enemyIndexY, int enemyIndexX);
private:
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<EnemyBase> pEnemy_[4];

	int mapData2[MAP_HEIGHT][MAP_WIDTH];

	// パワーエサの点滅
	int blendCount_;
	bool blendLimitMax_;

	// 表示するかどうか
	bool isDraw_;

	// パワーエサのハンドル
	int handle_;

	int pinkyGoalY_;
	int pinkyGoalX_;

	int blinkyGoalY_;
	int blinkyGoalX_;

	int InkyGoalY_;
	int InkyGoalX_;

	int crydeGoalY_;
	int crydeGoalX_;
};