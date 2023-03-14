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

	// �d�͉����x	�����Ȃ��ꍇ��0.0���w�肷��
	float gravity_;

	// �����x�̌������x	����255����w�肵���������Ă���
	int alphaDec_;

	// ���a�̕ω���
	float radiusAcc_;
};