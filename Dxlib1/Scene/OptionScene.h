#pragma once
#include "Scene.h"
/// <summary>
/// �I�v�V�����V�[��
/// </summary>
class OptionScene :public Scene
{
public:
	OptionScene(SceneManager& manager);

	~OptionScene() {}

	void Update(const InputState& input);
	void Draw();
private: 
	int handle_;
	int teachH_;
};

