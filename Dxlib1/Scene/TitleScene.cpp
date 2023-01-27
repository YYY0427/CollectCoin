#include "TitleScene.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "GameplayingScene.h"
#include "OptionScene.h"
#include <DxLib.h>
#include "../DrawFunctions.h"

void TitleScene::FadeInUpdate(const InputState& input)
{
				 //真っ黒から徐々に表示する場合場合
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
				//グラデーションを使って徐々に表示する場合
				/*fadeTimer_;*/
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &TitleScene::NormalUpdate;
		fadeValue_ = 0;
	}
}

void TitleScene::NormalUpdate(const InputState& input)
{
	//次へのボタンが押されたら次のシーンへ行く
	if (input.IsTriggered(InputType::next))
	{
		updateFunc_ = &TitleScene::FadeOutUpdate;
	}
	if (input.IsTriggered(InputType::prev))
	{
		manager_.ChangeScene(new OptionScene(manager_));
		return;
	}
}

void TitleScene::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval)
	{
		manager_.ChangeScene(new GameplayingScene(manager_));
		return;
	}
}

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::FadeInUpdate)
{
	titleH_ = my::MyLoadGraph(L"Data/img/title.png");
	gradH_ = my::MyLoadGraph(L"Data/img/gradetion.jpg");
	/*fadeTimer_ = 255;*/
}

TitleScene::~TitleScene()
{
	DeleteGraph(titleH_);
}

void TitleScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void TitleScene::Draw()
{
	DrawString(0, 0, L"TitleScene", 0xffffff, true);

	//普通の描画
	DrawRotaGraph(320,240,0.3,0.0,titleH_,true);

	//今から書く画像と、すでに描画されているスクリーンとの
	//ブレンドの仕方を指定している。
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, 640, 480, 0x000000, true);
	
	/*SetDrawBlendMode(DX_BLENDMODE_MULA, fadeValue_);
	//画面全体をテクスチャで塗りつぶす
	DrawGraph(0, 0, gradH_, true);*/

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}