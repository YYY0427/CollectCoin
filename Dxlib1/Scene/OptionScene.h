#pragma once
#include "Scene.h"
/// <summary>
/// オプションシーン
/// </summary>
class OptionScene :public Scene
{
public:
	OptionScene(SceneManager& manager);

	~OptionScene() {}

	void Update(const InputState& input);
	void Draw();
private: 
	int controllerH_;
	int optionH_;
	int teachH_;
	int noteH_;
	int speakerH_;
	int bigH_;
	int midleH_;
	int noneH_;
	int cursor1H_;
	int cursor2H_;
	int cursor3H_;
	int cursor4H_;

	int currentInputIndex_;
};

