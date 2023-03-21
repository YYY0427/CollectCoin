#include "BackGround.h"
#include "../Game.h"
#include "Player.h"
#include <DxLib.h>

BackGround::BackGround(int handle) :
	handle_(handle),
	scroll_(0)
{
	GetGraphSizeF(handle_, &size_.x, &size_.y);

	width_ = static_cast<int>(Game::SCREEN_WIDTH / size_.x);
	height_ = static_cast<int>(Game::SCREEN_HEIGHT / size_.y);
}

void BackGround::Update(bool getPowerFeed)
{
	if (!getPowerFeed)
	{
		scroll_ += 1;
		if (scroll_ >= Game::SCREEN_HEIGHT)
		{
			scroll_ = 0;
		}
	}
	else
	{
		scroll_ -= 2;
		if (scroll_ <= 0)
		{
			scroll_ = Game::SCREEN_HEIGHT;
		}
	}
}

void BackGround::Draw()
{
	for (int y = 0; y < height_ + 1; y++)
	{
		for (int x = 0; x < width_ + 1; x++)
		{
			DrawRotaGraph(static_cast<int>(x * size_.x), static_cast<int>(y * size_.y + scroll_), 1.0f, 0.0f, handle_, true);
			DrawRotaGraph(static_cast<int>(x * size_.x), static_cast<int>(y * size_.y - Game::SCREEN_HEIGHT + scroll_), 
				1.0f, 0.0f, handle_, true);
		}
	}
}