#pragma once
#include "../vec2.h"

class Coin
{
public:
	Coin(int graph);
	virtual ~Coin(){}

	void Update();
	void Draw();
	bool IsEnabled();
private:
	int imgH_;
	Vec2 pos_;
	float fallSpeed_;
	int coinImgIdx_;
};