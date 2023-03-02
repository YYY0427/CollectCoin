#pragma once
#include "EnemyBase.h"
#include "../vec2.h"
#include <memory>

class Field;
class Player;

/// <summary>
/// �S���^�C�v�G
/// </summary>
class Skeleton : public EnemyBase
{
public:
	// �摜�̕�
	static constexpr int WIDTH = 32;

	// �摜�̍���
	static constexpr int HEIGHT = 32;

	Skeleton(int handle, int indexX, int indexY);
	virtual ~Skeleton(){}

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;
private:
};