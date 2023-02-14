#pragma once
#include "EnemyBase.h"

/// <summary>
/// æ‰ñ‚è‚·‚é“G
/// </summary>
class PinkyEnemy : public EnemyBase
{
public:
	PinkyEnemy(int handle, int indexX, int indexY);

	/// <summary>
	/// XV
	/// </summary>
	virtual void Update()override;
};