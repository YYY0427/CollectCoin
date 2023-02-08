#pragma once
//「スマートポインタ」を使うためのinclude
#include <memory>	
#include "../Geometry.h"
#include "../vec2.h"

class Field;
class ChasingEnemy;
class InputState;

class Player
{
public:
	// コンストラクタ
	Player();

	//デストラクタ
	~Player(){};

	void SetChasingEnemy(std::shared_ptr<ChasingEnemy>chasingEnemy) { pChasingEnemy_ = chasingEnemy; }
	void SetField(std::shared_ptr<Field>field) { pField_ = field; }

	// 処理
	void Update(const InputState& input);

	// 描画
	void Draw();

	// 
	void DeadUpdate();

	// 当たり判定
	bool Colision(int direction);

	// パワーエサを取得した場合の速度計算
	void SpeedCalculation();

	// インデックス座標を座標に変換
	void PosCalculation();

	// プレイヤーの座標の取得
	Vec2 GetSize() const { return size_; }
	Vec2 GetPos() const { return pos_; }

	// パワーエサを取得したかどうか
	bool GetPowerFeed() const { return isPowerFeed_; }

	// 死亡設定
	void SetDead(bool isDead) { isDead_ = isDead; }

	// 死亡時のアニメーションの終了するかどうかを取得
	bool GetAnimeEnd() const { return isAnimeEnd_; }

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
	std::shared_ptr<ChasingEnemy> pChasingEnemy_;

	Vec2 size_;
	
	// プレイヤーの座標
	Vec2 pos_;

	//プレイヤーのインデックス座標
	int indexX_;
	int indexY_;

	// プレイヤーの仮の座標
	int kX_;
	int kY_;

	//プレイヤーの画像
	int handle_;
	int deathHandle_;

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
	int moveSpeed_;

	// パワーエサを取得したかどうか
	bool isPowerFeed_;
	
	// パワーエサを取得した状態でどのくらい経ったか
	int powerFeedTimer_;

	// パワーエサを取得した場合のスピード
	float powerFeedSpeed_;

	// プレイヤーが敵と当たったか
	bool isDead_;

	// 死亡時アニメーションを終了するか
	bool isAnimeEnd_;
};