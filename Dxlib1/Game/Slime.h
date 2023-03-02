#pragma once
#include "EnemyBase.h"

/// <summary>
/// ‹C‚Ü‚®‚ê‚È“G
/// </summary>
class Slime : public EnemyBase
{
public:
	// ‰æ‘œ‚Ì•
	static constexpr int WIDTH = 32;

	// ‰æ‘œ‚Ì‚‚³
	static constexpr int HEIGHT = 32;

	Slime(int handle, int indexX, int indexY);

	virtual ~Slime(){}
	/// <summary>
	/// XV
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;

private:
};