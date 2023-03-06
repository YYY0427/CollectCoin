#pragma once
#include "EnemyBase.h"

/// <summary>
/// �C�܂���ȓG
/// </summary>
class Slime : public EnemyBase
{
public:
	// �摜�̕�
	static constexpr int WIDTH = 32;

	// �摜�̍���
	static constexpr int HEIGHT = 32;

	// �摜�̊g�嗦
	static constexpr float SCALE = 2.0f;

	// 1���ɕK�v�ȃt���[����
	static constexpr int ANIME_FRAME_SPEED = 8;

	// �A�j���[�V��������
	static constexpr int ANIME_FRAME_NUM = 7;

	Slime(int handle, int indexX, int indexY);

	virtual ~Slime(){}
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;

private:
};