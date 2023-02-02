#pragma once
#include "EnemyBase.h"

/// <summary>
/// 粘着タイプ敵
/// </summary>
class ChasingEnemy : public EnemyBase
{
public:
	ChasingEnemy(std::shared_ptr<Player> player);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()override;
private:
	// 画像のハンドル
	int handle_;


	int idX_;
};

