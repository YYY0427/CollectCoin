#pragma once
#include "EnemyBase.h"

/// <summary>
/// �C�܂���ȓG
/// </summary>
class Slime : public EnemyBase
{
public:
	// �摜�̕�
	static constexpr int WIDTH = 32;

	// �摜�̍���
	static constexpr int HEIGHT = 32;

	Slime(int handle, int indexX, int indexY);

	virtual ~Slime(){}
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;

private:
};