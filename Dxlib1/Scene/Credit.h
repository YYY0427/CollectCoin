#pragma once
#include "Scene.h"
class Credit :  public Scene
{
public:
	Credit(SceneManager& manager);
	virtual ~Credit();
	void Update(const InputState& input);
	void Draw();
private:
	int controllerH_;
	int creditH_;
	int teachH_;
};

