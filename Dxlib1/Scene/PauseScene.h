#pragma once
#include "Scene.h"

class InputState;
class SceneManager;

/// <summary>
/// ポーズシーン
/// </summary>

class PauseScene : public Scene
{
public:
	PauseScene(SceneManager& manager);
	virtual ~PauseScene();

	virtual void Update(const InputState& input)override;

	void Draw();
private:
	int selectionH_;

	int pauseWidth_;

	int stringGraph_;

	double rate_;

	bool a_;
};

