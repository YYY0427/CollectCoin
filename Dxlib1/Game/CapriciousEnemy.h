#pragma once
#include "EnemyBase.h"

/// <summary>
/// �C�܂���ȓG
/// </summary>
class CapriciousEnemy : public EnemyBase
{
public:
	CapriciousEnemy(int handle, int indexX, int indexY);

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

private:
};

