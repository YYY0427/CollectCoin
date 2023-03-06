#pragma once
#include "EnemyBase.h"

/// <summary>
/// ���肷��G
/// </summary>
class Golem : public EnemyBase
{
public:
	// �摜�̕�
	static constexpr int WIDTH = 32;

	// �摜�̍���
	static constexpr int HEIGHT = 32;

	// �摜�̊g�嗦
	static constexpr float SCALE = 2.3f;

	// 1���ɕK�v�ȃt���[����
	static constexpr int ANIME_FRAME_SPEED = 10;

	// �A�j���[�V��������
	static constexpr int ANIME_FRAME_NUM = 4;

	Golem(int handle, int indexX, int indexY);

	virtual ~Golem(){}
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;
};