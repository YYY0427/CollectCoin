#pragma once
#include "EnemyBase.h"

/// <summary>
/// ‹C‚Ü‚®‚ê‚È“G
/// </summary>
class CapriciousEnemy : public EnemyBase
{
public:
	CapriciousEnemy(int handle, int indexX, int indexY);

	/// <summary>
	/// XV
	/// </summary>
	virtual void Update()override;

private:
};

