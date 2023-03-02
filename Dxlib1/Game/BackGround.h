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

	// ‚»‚ê‚¼‚ê‚Ì•ûŒü‚É•\¦‚·‚é–‡”
	int width_;
	int height_;

	int scroll_;
};