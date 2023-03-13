#include "BackGround.h"
#include "../Game.h"
#include "Player.h"
#include <DxLib.h>

BackGround::BackGround(int handle) :
	handle_(handle),
	scroll_(0)
{
	GetGraphSizeF(handle_, &size_.x, &size_.y);

	width_ = Game::kScreenWidth / size_.x;
	height_ = Game::kScreenHeight / size_.y;
}

void BackGround::Update()
{
	if (!pPlayer_->GetPowerFeed())
	{
		scroll_ += 1;
		if (scroll_ >= Game::kScreenHeight)
		{
			scroll_ = 0;
		}
	}
	else
	{
		scroll_ -= 2;
		if (scroll_ <= 0)
		{
			scroll_ = Game::kScreenHeight;
		}
	}
}

void BackGround::Draw()
{
	for (int y = 0; y < height_ + 1; y++)
	{
		for (int x = 0; x < width_ + 1; x++)
		{
			DrawRotaGraph(x * size_.x, y * size_.y + scroll_, 1.0f, 0.0f, handle_, true);
			DrawRotaGraph(x * size_.x, y * size_.y - Game::kScreenHeight + scroll_, 1.0f, 0.0f, handle_, true);
		}
	}
}