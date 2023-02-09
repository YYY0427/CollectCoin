#pragma once
#include "EnemyBase.h"

/// <summary>
/// ˆêŠÑ«‚ª‚È‚¢“G
/// </summary>
class InconsistentEnemy : public EnemyBase
{
public:
	InconsistentEnemy(int handle, int indexX, int indexY);

	/// <summary>
	/// XV
	/// </summary>
	virtual void Update()override;
};

