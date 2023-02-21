#pragma once
#include "EnemyBase.h"
#include "../vec2.h"
#include <memory>

class Field;
class Player;

/// <summary>
/// �S���^�C�v�G
/// </summary>
class BlinkyEnemy : public EnemyBase
{
public:
	BlinkyEnemy(int handle, int indexX, int indexY);
	virtual ~BlinkyEnemy(){}

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	virtual void SetInit()override;
private:
};