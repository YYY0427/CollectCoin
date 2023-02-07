#pragma once
#include "EnemyBase.h"
#include "../vec2.h"
#include <memory>

class Field;
class Player;
class GameplayingScene;

/// <summary>
/// 粘着タイプ敵
/// </summary>
class ChasingEnemy : public EnemyBase
{
public:
	ChasingEnemy(std::shared_ptr<Field>field, std::shared_ptr<Player>player);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()override;

	// 当たり判定
	bool Colision(int direction);

	// パワーエサを取得した場合の速度計算
	void SpeedCalculation();

	// インデックス座標を座標に変換
	void PosCalculation();

	// 敵の座標の取得
	Vec2 GetSize()const { return size_; }
	Vec2 GetPos() const { return pos_; }

	void SetEnabled(bool isEnabled) { isEnabled_ = isEnabled; }

	/// <summary>
	/// 向いている方向によって値を返す
	/// </summary>
	/// <returns>画像のy座標</returns>
	virtual int DirectReturnNum();
private:
	std::shared_ptr<Field> pField_ = nullptr;
	std::shared_ptr<Player> pPlayer_ = nullptr;

	Vec2 size_;
	Vec2 pos_;

	int idY_;

	int izikeHandle_;

	// プレイヤーの仮の座標
	int kX_;
	int kY_;

	// 移動インターバル用タイマー
	int moveTimer_;

	// プレイヤーがどの方向に進みたいか
	int wantMoveDirection_;

	// プレイヤーの移動速度
	int moveSpeed_;

	// パワーエサを取得した場合のスピード
	float powerFeedSpeed_;

	// パワーエサを取得したかどうか
	bool isPowerFeed_;

	// パワーエサを取得した状態でどのくらい経ったか
	int powerFeedTimer_;

	bool isEnabled_;

	int flashingImgY_;
};

