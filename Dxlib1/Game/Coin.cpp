#include "Coin.h"
#include "../game.h"
#include <DxLib.h>

namespace
{
	// アニメーション
	constexpr int COIN_FRAME_NUM = 4;
	constexpr int COIN_FRAME_SPEED = 15;
}

Coin::Coin(int graph):
	imgH_(graph),
	pos_(GetRand(Game::kScreenWidth), 0),
	fallSpeed_(static_cast<float>(GetRand(3.0f) + 2.0f)),
	coinImgIdx_(0)
{
}

void Coin::Update()
{
	// アニメーション処理
	coinImgIdx_ = (coinImgIdx_ + 1) % (COIN_FRAME_SPEED * COIN_FRAME_NUM);
	pos_.y += fallSpeed_;
}

void Coin::Draw()
{
	int imgX = (coinImgIdx_ / COIN_FRAME_SPEED) * 8;
	DrawRectRotaGraph(pos_.x, pos_.y, imgX, 0, 8, 8, 3.0f, 0.0f, imgH_, true);
}
