#pragma once
#include "EnemyBase.h"

/// <summary>
/// 気まぐれな敵
/// </summary>
class Slime : public EnemyBase
{
public:
	// 画像の幅
	static constexpr int WIDTH = 32;

	// 画像の高さ
	static constexpr int HEIGHT = 32;

	// 画像の拡大率
	static constexpr float SCALE = 2.0f;

	// 1枚に必要なフレーム数
	static constexpr int ANIME_FRAME_SPEED = 8;

	// アニメーション枚数
	static constexpr int ANIME_FRAME_NUM = 7;

	Slime(int handle, int indexX, int indexY);

	virtual ~Slime(){}
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;

private:
};