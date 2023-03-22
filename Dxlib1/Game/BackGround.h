#pragma once
#include "../vec2.h"
#include <memory>

class Player;

/// <summary>
/// 背景
/// </summary>
class BackGround
{
public:
	// コンストラクタ
	BackGround(int handle);

	// デストラクタ
	virtual~BackGround(){};

	// 初期化処理
	void Init();

	// 更新処理
	void Update(bool getPowerFeed);

	// 描画処理
	void Draw();

	// プレイヤーのポインタの設定
	void SetPlayer(std::shared_ptr<Player>player) { pPlayer_ = player; }
private:
	// プレイヤー
	std::shared_ptr<Player> pPlayer_;

	// 画像ハンドル
	int handle_;

	// 画像サイズ
	Vec2 size_;

	// それぞれの方向に表示する枚数
	int width_;
	int height_;

	// スクロール
	int scroll_;
};