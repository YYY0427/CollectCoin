#pragma once
#include "EnemyBase.h"

/// <summary>
/// ��ѐ����Ȃ��G
/// </summary>
class CrydeEnemy : public EnemyBase
{
public:
	CrydeEnemy(int handle, int indexX, int indexY);

	virtual ~CrydeEnemy(){}

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;
};

