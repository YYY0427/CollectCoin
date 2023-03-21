#include "PauseScene.h"
#include "DxLib.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "../Game.h"
#include <cmath>

namespace
{
	constexpr int WS_WIDTH = Game::SCREEN_WIDTH;	// ��
	constexpr int WS_HEIGHT = Game::SCREEN_HEIGHT;	// ����
	constexpr int WS_START_Y = 0;					// �J�n�ʒuy���W
	constexpr int WS_START_X = 0;					// �J�n�ʒux���W

	// �|�[�Y���̃��b�Z�[�W
	const char* const PAUSE_STRING = "PAUSED";
	constexpr int FONT_SIZE = 80;
}

PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager),
	rate_(0.0),
	isPlusOrMinus(false),
	pauseWidth_(0),
	selectionH_(-1),
	stringGraphH_(-1)
{
	Init();
}

PauseScene::~PauseScene()
{
	End();
}

void PauseScene::Init()
{
	// �����̊g�嗦�̏�����
	rate_ = 0.75;

	// �t�H���g�̃��[�h
	selectionH_ = CreateFontToHandle("PixelMplus10", FONT_SIZE, 9);

	// ������̕��̎擾
	pauseWidth_ = GetDrawStringWidthToHandle(PAUSE_STRING, static_cast<int>(strlen(PAUSE_STRING)), selectionH_);

	// ��������摜��
	stringGraphH_ = MakeScreen(pauseWidth_, FONT_SIZE, true);
	SetDrawScreen(stringGraphH_);
	DrawStringToHandle(0, 0, PAUSE_STRING, 0xffffff, selectionH_, false);
	SetDrawScreen(DX_SCREEN_BACK);
}

void PauseScene::End()
{
	DeleteFontToHandle(selectionH_);
	DeleteGraph(stringGraphH_);
}

void PauseScene::Update(const InputState& input)
{
	// �����̊g��k������
	if (rate_ > 1.0)
	{
		isPlusOrMinus = false;
	}
	else if (rate_ < 0.75)
	{
		isPlusOrMinus = true;
	}
	if (isPlusOrMinus)
	{
		rate_ += 0.005;
	}
	else
	{
		rate_ -= 0.005;
	}

	// �߂�
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PopScene();
		return;
	}
}

void PauseScene::Draw()
{
	// �e
 	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	DrawBox(WS_START_Y, WS_START_X, WS_START_Y + WS_WIDTH, WS_START_X + WS_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	

	// �|�[�Y������̕\��
	DrawRotaGraph(((WS_START_Y + WS_WIDTH) / 2) - (pauseWidth_ / 2) + 130, (WS_START_X + WS_HEIGHT) / 2 - 200,
		rate_, 0.0f, stringGraphH_, true, false);
}