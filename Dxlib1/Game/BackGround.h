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

	// ���ꂼ��̕����ɕ\�����閇��
	int width_;
	int height_;

	int scroll_;
};