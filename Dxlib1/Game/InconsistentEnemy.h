#pragma once
#include "EnemyBase.h"

/// <summary>
/// ��ѐ����Ȃ��G
/// </summary>
class InconsistentEnemy : public EnemyBase
{
public:
	InconsistentEnemy(int handle, int indexX, int indexY);

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;
};

