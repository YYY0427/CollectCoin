#pragma once
#include <memory>
#include <vector>
#include "EnemyBase.h"

class Player;
class EnemyBase;

class Field
{
public:
	// ブロックの大きさ
	static constexpr int CHIP_SIZE = 32;

	// 表示場所
	static constexpr int DISPLAY_POS_X = 495;
	static constexpr int DISPLAY_POS_Y = 100;

	// ステージのサイズの取得
	int GetMapDataY() const { return mapHeight_; }
	int GetMapDataX() const { return mapWidth_; }

	// コンストラクタ
	Field(int sordH, int doorH, int coinH, int stage);
	 
	// デストラクタ
	virtual ~Field();

	// プレイヤー死亡時の初期化
	void Init();

	// ステージによって配列のサイズの変更、敵の目標を変更
	void StageCheck(int stage);
	void StageCheck2(int stage);

	// 更新処理
	void Updata();

	// 描画処理
	void Draw();
	
	// ポインタの設定
	void SetEnemy(std::shared_ptr<EnemyBase>enemy, int i) { pEnemy_[i] = enemy; }
	void SetPlayer(std::shared_ptr<Player>player) { pPlayer_ = player; }

	// マップデータのコインの合計枚数の集計
	int LestCoin();

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
	int  Warp(int ky, int kx, int x, int y);

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

	// 敵の動き
	int SkeletonMove(int enemyIndexY, int enemyIndexX, bool flag);
	int GolemMove(int enemyIndexY, int enemyIndexX, bool flag);
	int SlimeMove(int enemyIndexY, int enemyIndexX, bool flag);
	int GhostMove(int enemyIndexY, int enemyIndexX, bool flag);
private:
	std::shared_ptr<Player> pPlayer_;
	std::vector<std::shared_ptr<EnemyBase>> pEnemy_;
	std::vector<std::vector<int>> mapData_;
	std::vector<std::vector<int>> enemyTargetMapData_;

	// ステージのサイズ
	int mapHeight_;
	int mapWidth_;
	
	// ステージ
	int stage_;

	// 表示するかどうか
	bool isDraw_;

	// プレイヤーが取得したコインの枚数
	int getCoinNum_;

	// プレイヤーが集めなければいけないコインの枚数
	int coinTotalNum_;

	// ドアを開くか閉めるか
	bool openDoor_;
	bool closeDoor_;

	// 画像のハンドル
	int coinH_;
	int sordH_;
	int doorH_;

	// 画像インデックス
	int coinImgIdx_;
	int doorImgIdx_;

	// フォントハンドル
	int stringH_;

	// 敵の目標
	int pinkyGoalY_;
	int pinkyGoalX_;

	int blinkyGoalY_;
	int blinkyGoalX_;

	int inkyGoalY_;
	int inkyGoalX_;

	int crydeGoalY_;
	int crydeGoalX_;
};