#include "Shot.h"
#include <DxLib.h>

Shot::Shot(int handle) :
	handle_(handle),
	vel_(6.0f, 0.0f)
{
}

void Shot::Fire(const Position2& pos)
{
	pos_ = pos;
	isEnabled_ = true;
}

bool Shot::IsEnabled() const
{
	return isEnabled_;
}

void Shot::Update()
{
	pos_ += vel_;
	imgIdx_ = (imgIdx_ + 1) % 2;

	if (pos_.x < -19 ||640 + 19 < pos_.x )
	{
		isEnabled_ = false;
	}
}

void Shot::Draw()
{
	DrawRectRotaGraph(
		pos_.x, pos_.y,
		imgIdx_ * 19, 0,
		19, 6,
		3.0f, 0.0f,
		handle_, true);
}
