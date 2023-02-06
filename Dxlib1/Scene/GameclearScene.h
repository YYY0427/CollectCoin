#pragma once
#include "Scene.h"

class GameclearScene : public Scene
{
public:
	GameclearScene(SceneManager& manager);
	~GameclearScene() {}

	void Update(const InputState& input);
	void Draw();
private:
	int gameOverH_ = -1;

	static constexpr int fade_interval = 60;

	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	unsigned int fadeColor_ = 0xff0000;

	void FadeInUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);
	void FadeOutUpdate(const InputState& input);

	using UpdateFucn_t = void (GameclearScene::*)(const InputState&);
	UpdateFucn_t updateFunc_;
};