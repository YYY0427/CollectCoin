#pragma once
#include "EnemyBase.h"

/// <summary>
/// ��ѐ����Ȃ��G
/// </summary>
class Ghost : public EnemyBase
{
public:
	// �摜�̕�
	static constexpr int WIDTH = 25;

	// �摜�̍���
	static constexpr int HEIGHT = 25;

	// �摜�̊g�嗦
	static constexpr float SCALE = 2.0f;

	// 1���ɕK�v�ȃt���[����
	static constexpr int ANIME_FRAME_SPEED = 10;

	// �A�j���[�V��������
	static constexpr int ANIME_FRAME_NUM = 4;

	Ghost(int handle, int indexX, int indexY);

	virtual ~Ghost(){}

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;
};

