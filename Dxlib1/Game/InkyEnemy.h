#pragma once
#include "EnemyBase.h"

/// <summary>
/// ‹C‚Ü‚®‚ê‚È“G
/// </summary>
class InkyEnemy : public EnemyBase
{
public:
	InkyEnemy(int handle, int indexX, int indexY);

	/// <summary>
	/// XV
	/// </summary>
	virtual void Update()override;

	void MoveSwitch();
private:
};