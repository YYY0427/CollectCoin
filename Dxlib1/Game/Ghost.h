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

	Ghost(int handle, int indexX, int indexY);

	virtual ~Ghost(){}

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;
};

