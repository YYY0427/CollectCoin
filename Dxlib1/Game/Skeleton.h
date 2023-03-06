#pragma once
#include "EnemyBase.h"
#include "../vec2.h"
#include <memory>

class Field;
class Player;

/// <summary>
/// �S���^�C�v�G
/// </summary>
class Skeleton : public EnemyBase
{
public:
	// �摜�̕�
	static constexpr int WIDTH = 32;

	// �摜�̍���
	static constexpr int HEIGHT = 32;

	// �摜�̊g�嗦
	static constexpr float SCALE = 2.0f;

	// 1���ɕK�v�ȃt���[����
	static constexpr int ANIME_FRAME_SPEED = 10;

	// �A�j���[�V��������
	static constexpr int ANIME_FRAME_NUM = 4;

	Skeleton(int handle, int indexX, int indexY);
	virtual ~Skeleton(){}

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;
private:
};