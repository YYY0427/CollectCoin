#include "BackGround.h"
#include "../Game.h"
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
	scroll_ += 1;
	if (scroll_ >= Game::kScreenHeight)
	{
		scroll_ = 0;
	}
}

void BackGround::Draw()
{
	for (int z = 0; z < 2; z++)
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
}