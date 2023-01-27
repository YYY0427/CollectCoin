#include "OptionScene.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include <DxLib.h>

void OptionScene::Update(const InputState& input)
{
	//次へのボタンが押されたら次のシーンへ行く
	if (input.IsTriggered(InputType::prev))
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
}

void OptionScene::Draw()
{
	DrawString(300, 200, L"OptionScene", GetColor(255, 255, 255));
}