#pragma once
#include "EnemyBase.h"

/// <summary>
/// ��ѐ����Ȃ��G
/// </summary>
class CrydeEnemy : public EnemyBase
{
public:
	CrydeEnemy(int handle, int indexX, int indexY);

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;
};

