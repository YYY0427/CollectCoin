#pragma once
#include "EnemyBase.h"

/// <summary>
/// ���肷��G
/// </summary>
class PinkyEnemy : public EnemyBase
{
public:
	PinkyEnemy(int handle, int indexX, int indexY);

	virtual ~PinkyEnemy(){}
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;
};