#include "Particle.h"
#include <DxLib.h>

Particle::Particle() :
	pos_(0, 0),
	vec_(0, 0),
	radius_(2.0f),
	color_(0xffffff),
	isExist_(false),
	gravity_(2.0f),
	alphaDec_(4),
	radiusAcc_(0.0f),
	alpha_(255)
{

}

void Particle::SetPos(Vec2 pos)
{
	pos_ = pos;
}

void Particle::Update()
{
	randSin_ = static_cast<float>(GetRand(360)) / 360.0f;
	randSin_ *= DX_TWO_PI_F;
	randSpeed_ = static_cast<float>(GetRand(160)) / 10.0f + 1.0f;

	pos_.x = pos_.x + cosf(randSin_) * 2.0f;
	pos_.y = pos_.y + sinf(randSin_) * 2.0f;

	vec_.x = cosf(randSin_) * randSpeed_;
	vec_.y = sinf(randSin_) * randSpeed_;

	pos_ += vec_;
	vec_.y += gravity_;
}

void Particle::Draw()
{
	DrawCircleAA(pos_.x, pos_.y, radius_, 16, color_, true);
}
