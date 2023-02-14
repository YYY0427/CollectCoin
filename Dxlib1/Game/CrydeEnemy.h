#pragma once
#include "EnemyBase.h"

/// <summary>
/// ˆêŠÑ«‚ª‚È‚¢“G
/// </summary>
class CrydeEnemy : public EnemyBase
{
public:
	CrydeEnemy(int handle, int indexX, int indexY);

	/// <summary>
	/// XV
	/// </summary>
	virtual void Update()override;
};

