#pragma once
#include "../vec2.h"

class BackGround
{
public:
	BackGround(int handle);
	virtual~BackGround(){};

	void Update();

	void Draw();

private:
	int handle_;

	Vec2 size_;

	// それぞれの方向に表示する枚数
	int width_;
	int height_;

	int scroll_;
};