#pragma once
#include "EnemyBase.h"

/// <summary>
/// ���肷��G
/// </summary>
class PreemptiveEnemy : public EnemyBase
{
public:
	PreemptiveEnemy(int handle, int indexX, int indexY);

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;
};

