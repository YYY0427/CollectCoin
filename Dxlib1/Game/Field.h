#pragma once
#include <memory>
#include "EnemyBase.h"

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
	static constexpr int CHIP_SIZE = 32;

	// 表示場所
	static constexpr int DISPLAY_POS_X = 400;
	static constexpr int DISPLAY_POS_Y = 100;

	// コンストラクタ
	Field();
	 
	// デストラクタ
	virtual ~Field(){}

	void Init();

	// 更新処理
	void Updata();

	// 描画処理
	void Draw();
	
	// ポインタの設定
	void SetEnemy(std::shared_ptr<EnemyBase>enemy, int i) { pEnemy_[i] = enemy; }
	void SetPlayer(std::shared_ptr<Player>player) { pPlayer_ = player; }

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

	// 目的地までの最短経路を探すための準備として、コピーマップの初期化と目的地の場所に0(目印)を設定
	void MoveDataSet(int playerY, int playerX);

	// フィールドにプレイヤーからの距離を求める関数
	void Search(int y, int x, int goalY, int goalX, int pos);

	// 敵が特定の場所にいる場合敵の足を遅くするフラグを立てる関数
	bool SlowPosition(int y,int x);

	// 敵のスポーン地点の外にいるか中にいるか
	bool SpornInOrAuto(int y, int x);

	// 特定のブロックに侵入可能か
	bool Intrusion(int y, int x, bool flag);

	// mapData2を参照してプレイヤーからの数字が小さい方向の値を返す 
	// プレイヤーがパワーエサを取得していたら大きい方向の値を返す
	int BlinkyMove(int enemyIndexY, int enemyIndexX, bool flag);

	int PinkyMove(int enemyIndexY, int enemyIndexX, bool flag);

	int InkyMove(int enemyIndexY, int enemyIndexX, bool flag);

	int CrydeMove(int enemyIndexY, int enemyIndexX, bool flag);
private:
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<EnemyBase> pEnemy_[EnemyBase::enemy_num];

	int mapData2[Field::MAP_HEIGHT][Field::MAP_WIDTH];

	// 表示するかどうか
	bool isDraw_;

	// 画像のハンドル
	int handle_;
	int coinH_;
	int sordH_;

	int imgIdX_;
	int sordIdx_;

	int pinkyGoalY_;
	int pinkyGoalX_;

	int blinkyGoalY_;
	int blinkyGoalX_;

	int inkyGoalY_;
	int InkyGoalX_;

	int crydeGoalY_;
	int crydeGoalX_;
};