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
	/// <summary>
	/// 敵オブジェクトの更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 敵オブジェクトの描画
	/// </summary>
	virtual void Draw() = 0;

protected:	// 子クラスには見せたい情報

	enum DIRECT
	{
		up = 1,
		down,
		left,
		right,
		direct_num
	};

	std::shared_ptr<Player> player_= nullptr;

	// 画像のハンドル
	int handle_;

	// 表示する画像のインデックス
	int idX_;

	// 動いている方向
	int moveDirection_;

	// 座標　
	int posX_;
	int posY_;

	// インデックス座標
	int indexX_;
	int indexY_;
};
