#pragma once
#include "EnemyBase.h"
#include "../vec2.h"
#include <memory>

class Field;
class Player;

/// <summary>
/// 粘着タイプ敵
/// </summary>
class BlinkyEnemy : public EnemyBase
{
public:
	BlinkyEnemy(int handle, int indexX, int indexY);
	virtual ~BlinkyEnemy(){}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()override;

	virtual void SetInit()override;
private:
};