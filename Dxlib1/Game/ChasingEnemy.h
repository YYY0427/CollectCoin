#pragma once
#include "EnemyBase.h"

/// <summary>
/// �S���^�C�v�G
/// </summary>
class ChasingEnemy : public EnemyBase
{
public:
	ChasingEnemy(std::shared_ptr<Player> player);

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw()override;
private:
	// �摜�̃n���h��
	int handle_;


	int idX_;
};

