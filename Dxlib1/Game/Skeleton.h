#pragma once
#include "EnemyBase.h"
#include "../vec2.h"
#include <memory>

class Field;
class Player;

/// <summary>
/// 粘着タイプ敵
/// </summary>
class Skeleton : public EnemyBase
{
public:
	// 画像の幅
	static constexpr int WIDTH = 32;

	// 画像の高さ
	static constexpr int HEIGHT = 32;

	Skeleton(int handle, int indexX, int indexY);
	virtual ~Skeleton(){}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;
private:
};