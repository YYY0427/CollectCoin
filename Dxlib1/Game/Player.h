#pragma once
//「スマートポインタ」を使うためのinclude
#include <memory>	
#include "../Geometry.h"

class Field;
class InputState;

class Player
{
public:
	// コンストラクタ
	Player();

	//デストラクタ
	~Player(){};

	// 処理
	void Update(const InputState& input);

	// 描画
	void Draw();

	// 当たり判定
	bool Colision(int direction);

	// パワーエサを取得した場合の速度計算
	void SpeedCalculation();

	// インデックス座標を座標に変換
	void PosCalculation();

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

	//フィールド
	std::shared_ptr<Field> pField_;

	// 自機の現在の座標
	Position2 pos_;	

	// プレイヤーの座標
	int posX_;
	int posY_;

	//プレイヤーのインデックス座標
	int indexX_;
	int indexY_;

	// プレイヤーの仮の座標
	int kX_;
	int kY_;

	//プレイヤーの画像
	int handle_;

	// 移動インターバル用タイマー
	int moveTimer_;

	// 表示する画像のインデックス
	int imgIdX_;	

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

	float powerFeedSpeed_;
};