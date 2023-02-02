#pragma once
#include <memory>

// プロトタイプ宣言
class Player;

/// <summary>
/// 敵の基底クラス
/// </summary>
class EnemyBase
{
public:
	EnemyBase(std::shared_ptr<Player> player);

	/// <summary>
	/// 敵オブジェクトの更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 敵オブジェクトの描画
	/// </summary>
	virtual void Draw() = 0;
protected:	// 子クラスには見せたい情報
	std::shared_ptr<Player> player_= nullptr;
};
