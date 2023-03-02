#pragma once
#include "EnemyBase.h"

/// <summary>
/// ˆêŠÑ«‚ª‚È‚¢“G
/// </summary>
class Ghost : public EnemyBase
{
public:
	// ‰æ‘œ‚Ì•
	static constexpr int WIDTH = 25;

	// ‰æ‘œ‚Ì‚‚³
	static constexpr int HEIGHT = 25;

	Ghost(int handle, int indexX, int indexY);

	virtual ~Ghost(){}

	/// <summary>
	/// XV
	/// </summary>
	virtual void Update()override;

	virtual void Draw()override;

	virtual void SetInit()override;
};

