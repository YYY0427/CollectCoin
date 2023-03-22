#include "Coin.h"
#include "../game.h"
#include <DxLib.h>

namespace
{
	// アニメーション
	constexpr int COIN_FRAME_NUM = 4;		// 必要枚数
	constexpr int COIN_FRAME_SPEED = 15;	// 速度
}

Coin::Coin(int graph):
	imgH_(graph),
	pos_(static_cast<float>(GetRand(Game::SCREEN_WIDTH)), 0.0f),
	fallSpeed_(GetRand(1) + 2.0f),
	coinImgIdx_(0)
{
}

void Coin::Init()
{
	pos_ = { static_cast<float>(GetRand(Game::SCREEN_WIDTH)), 0.0f };
	fallSpeed_ = GetRand(1) + 2.0f;
	coinImgIdx_ = 0;
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
	DrawRectRotaGraph(static_cast<int>(pos_.x), static_cast<int>(pos_.y), imgX, 0, 8, 8, 3.0f, 0.0f, imgH_, true);
}

bool Coin::IsEnabled()
{
	if (pos_.y > Game::SCREEN_HEIGHT)
	{
		return true;
	}
	return false;
}
