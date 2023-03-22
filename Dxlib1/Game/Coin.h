#pragma once
#include "../vec2.h"

class Coin
{
public:
	// コンストラクタ
	Coin(int graph);

	// デストラクタ
	virtual ~Coin(){}

	// 初期化処理
	void Init();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// コインの存在
	bool IsEnabled();
private:
	// コインの座標
	Vec2 pos_;

	// 画像ハンドル
	int imgH_;

	// コインの落下スピード
	float fallSpeed_;

	// コインの画像インデックス
	int coinImgIdx_;
};