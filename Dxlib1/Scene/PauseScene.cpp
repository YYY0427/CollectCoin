#include "PauseScene.h"
#include "DxLib.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "../Game.h"
#include <cmath>

namespace
{
	// �|�[�Y���̃��b�Z�[�W
	const char* const PAUSE_STRING = "PAUSED";
}

PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager),
	rate_(0.75),
	a_(false)
{
	selectionH_ = CreateFontToHandle("PixelMplus10", 25, 10);

	// ������̕��̎擾
	pauseWidth_ = GetDrawStringWidthToHandle(PAUSE_STRING, strlen(PAUSE_STRING), selectionH_);

	// ��������摜��
	stringGraph_ = MakeScreen(pauseWidth_, 25, true);
	SetDrawScreen(stringGraph_);
	DrawStringToHandle(0, 0, PAUSE_STRING, 0xffffff, selectionH_, false);
	SetDrawScreen(DX_SCREEN_BACK);
}

PauseScene::~PauseScene()
{
	DeleteFontToHandle(selectionH_);
}

void PauseScene::Update(const InputState& input)
{
	if (rate_ > 1.0)
	{
		a_ = false;
	}
	else if (rate_ < 0.75)
	{
		a_ = true;
	}
	if (a_)
	{
		rate_ += 0.01;
	}
	else
	{
		rate_ -= 0.01;
	}

	if (input.IsTriggered(InputType::pause))
	{
		manager_.PopScene();
		return;
	}
}

void PauseScene::Draw()
{
	//constexpr int pw_width = 400	//�|�[�Y�g�̕�
	//constexpr int pw_height = 300;	//�|�[�Y�g�̍���
	//constexpr int pw_start_x = (640 - pw_width) / 2;	//�|�[�Y�g�̍�
	//constexpr int pw_start_y = (480 - pw_height) / 2;	//�|�[�Y�g��

	constexpr int pw_width = Game::kScreenWidth;	// �|�[�Y�g�̕�
	constexpr int pw_height = Game::kScreenHeight;	// �|�[�Y�g�̍���
	constexpr int pw_start_x = 0;					// �|�[�Y�g�̍�
	constexpr int pw_start_y = 0;					// �|�[�Y�g��

 	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	

	// �|�[�Y�E�B���h�E�g��
//	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);

	DrawRotaGraph(((pw_start_x + pw_width) / 2) - (pauseWidth_ / 2) + 40, (pw_start_y + pw_height) / 2 + 45,
		rate_, 0.0f, stringGraph_, true, false);
}

