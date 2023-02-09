#pragma once
#include "EnemyBase.h"

/// <summary>
/// æ‰ñ‚è‚·‚é“G
/// </summary>
class PreemptiveEnemy : public EnemyBase
{
public:
	PreemptiveEnemy(int handle, int indexX, int indexY);

	/// <summary>
	/// XV
	/// </summary>
	virtual void Update()override;
};

