#pragma once
#include "EnemyBase.h"

/// <summary>
/// 一貫性がない敵
/// </summary>
class Ghost : public EnemyBase
{
public:
	// 画像の幅
	static constexpr int WIDTH = 25;

	// 画像の高さ
	static constexpr int HEIGHT = 25;

	// 画像の拡大率
	static constexpr float SCALE = 2.0f;

	// 1枚に必要なフレーム数
	static constexpr int ANIME_FRAME_SPEED = 10;

	// アニメーション枚数
	static constexpr int ANIME_FRAME_NUM = 4;

	Ghost(int handle, int indexX, int indexY);

	virtual ~Ghost(){}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;
};

