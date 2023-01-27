#pragma once
#include "../Geometry.h"

class Enemy;
class Field;
class InputState;

class Player
{
public:
	// コンストラクタ
	Player();

	//デストラクタ
	~Player();

	// 処理
	void Update(const InputState& input);

	// 描画
	void Draw();

	// アニメーション
	void Animation();
private:
	Enemy* pEnemy_ = nullptr;
	Field* pField_ = nullptr;

	// 自機の現在の座標
	Position2 pos_;	

	//プレイヤーの座標
	int indexX_;
	int indexY_;

	// プレイヤーの仮の座標
	int kX_;
	int kY_;

	//プレイヤーが右もしくは下に移動しているか
	bool isMoveDownOrRight;

	//プレイヤーの画像
	int handle_;

	// 移動インターバル用タイマー
	int moveTimer_;

	// 画像の切り替え用タイマー
	int animeTimer_;

	// 表示する画像のインデックス
	int imgIdx = 0;	

	// 画像の方向の切り替え
	float angle_;

	//プレイヤーがどの方向に移動しているか
	int moveDirection;
};