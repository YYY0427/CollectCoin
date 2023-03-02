#pragma once
#include "EnemyBase.h"

/// <summary>
/// æ‰ñ‚è‚·‚é“G
/// </summary>
class Golem : public EnemyBase
{
public:
	// ‰æ‘œ‚Ì•
	static constexpr int WIDTH = 32;

	// ‰æ‘œ‚Ì‚‚³
	static constexpr int HEIGHT = 32;

	Golem(int handle, int indexX, int indexY);

	virtual ~Golem(){}
	/// <summary>
	/// XV
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;
};