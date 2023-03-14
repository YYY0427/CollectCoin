#pragma once
#include "vec2.h"
class Particle
{
public:
	Particle();
	virtual ~Particle(){}

	void SetPos(Vec2 pos);

	void Update();

	void Draw();
private:
	Vec2 pos_;
	Vec2 vec_;
	float radius_;
	int alpha_;
	unsigned int color_;
	bool isExist_;
	float randSin_;
	float randSpeed_;

	// 重力加速度	落ちない場合は0.0を指定する
	float gravity_;

	// 透明度の減衰速度	初期255から指定した数減っていく
	int alphaDec_;

	// 半径の変化量
	float radiusAcc_;
};