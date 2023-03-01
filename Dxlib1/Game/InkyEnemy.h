#pragma once
#include "EnemyBase.h"

/// <summary>
/// ‹C‚Ü‚®‚ê‚È“G
/// </summary>
class InkyEnemy : public EnemyBase
{
public:
	InkyEnemy(int handle, int indexX, int indexY);

	virtual ~InkyEnemy(){}
	/// <summary>
	/// XV
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;

private:
};