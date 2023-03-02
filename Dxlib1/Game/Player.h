#pragma once
#include <memory>	
#include "../Geometry.h"
#include "../vec2.h"
#include "EnemyBase.h"

class Field;
class InputState;
class EnemyBase;

class Player
{
public:
	// コンストラクタ
	Player(int normalH, int waponH, int deadH, int indexX, int indexY);

	//デストラクタ
	~Player(){};

	// ポインタの設定
	void SetEnemy(std::shared_ptr<EnemyBase>enemy, int i) { pEnemy_[i] = enemy; }
	void SetField(std::shared_ptr<Field>field) { pField_ = field; }

	void Init();

	// 処理
	void Update(const InputState& input);

	// 敵が死んだ場合の処理 
	void DeadUpdate();

	// 描画
	void Draw();

	// 当たり判定
	bool Colision(int direction);

	// パワーエサを取得した場合の速度計算
	void SpeedCalculation();

	// インデックス座標を座標に変換
	void PosCalculation();

	// プレイヤーの座標の取得
	Vec2 GetSize() const { return deathImgSize_; }
	Vec2 GetPos() const { return pos_; }

	int GetIndexX() const { return indexX_; }

	int GetIndexY() const { return indexY_; }

	// パワーエサを取得したかどうか
	bool GetPowerFeed() const { return isPowerFeed_; }

	bool GetDead() const { return isDead_; }

	// 死亡設定
	void SetDead(bool isDead) { isDead_ = isDead; }

	// 存在設定
	void SetEnabled(bool isEnabled) { isEnabled_ = isEnabled; }

	// 死亡時のアニメーションの終了するかどうかを取得
	bool GetAnimeEnd() const { return isAnimeEnd_; }

	// 方向
	enum Direct
	{
		// 1からカウント
		up = 1,
		down,
		left,
		right,
		directNum
	};

private:

	// ポインタ
	std::shared_ptr<Field> pField_;	
	std::shared_ptr<EnemyBase> pEnemy_[EnemyBase::enemy_num];

	Vec2 deathImgSize_;
	
	// プレイヤーの座標
	Vec2 pos_;

	//プレイヤーのインデックス座標
	int indexX_;
	int indexY_;

	// プレイヤーの仮の座標
	int kX_;
	int kY_;

	bool isTurnFlag_;
	bool isFlash_;

	int timer_;

	//プレイヤーの画像
	int handle_;
	int normalH_;
	int waponH_;
	int deathH_;
	
	// 移動インターバル用タイマー
	int moveTimer_;

	// 表示する画像のインデックス
	int imgIdX_;
	int deadImgIdx_;

	// 画像の方向の切り替え
	float angle_;

	// プレイヤーがどの方向に進んでいるか
	int moveDirection_;

	// プレイヤーがどの方向に進みたいか
	int wantMoveDirection_;

	// 取得したエサの数
	int feedGetNum_;

	// プレイヤーの移動速度
	int moveInterval_;

	// パワーエサを取得したかどうか
	bool isPowerFeed_;

	// パワーエサを取得した状態でパワーエサを取得したか
	bool isPowerFeed2_;
	
	// パワーエサを取得した状態でどのくらい経ったか
	int powerFeedTimer_;

	// パワーエサを取得した場合のスピード
	float speed2_;

	// プレイヤーが敵と当たったか
	bool isDead_;

	// 死亡時アニメーションを終了するか
	bool isAnimeEnd_;

	float speed_;

	bool isEnabled_;

	// 特定のブロックに侵入可能か
	bool isIntrusion_;

	int imgY_;
};