#include "PauseScene.h"
#include "DxLib.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "KeyConfigScene.h"
#include "../Game.h"

PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager)
{
	selectionH_ = CreateFontToHandle(NULL, 15, 10);
}

void PauseScene::Update(const InputState& input)
{
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

	constexpr int pw_width = Game::kScreenWidth;	//�|�[�Y�g�̕�
	constexpr int pw_height = Game::kScreenHeight;	//�|�[�Y�g�̍���
	constexpr int pw_start_x = 0;	//�|�[�Y�g�̍�
	constexpr int pw_start_y = 0;	//�|�[�Y�g��

//	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
 	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	//�|�[�Y�E�B���h�E�Z���t�@��(����)
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//�ʏ�`��ɖ߂�

	//�|�[�Y�E�B���h�E�g��
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);

	//�|�[�Y�����b�Z�[�W
	int width1 = GetDrawStringWidthToHandle(L"PAUSED", 6, selectionH_);
	DrawStringToHandle(((pw_start_x + pw_width) / 2) - (width1 / 2), (pw_start_y + pw_height) / 2 + 40,
		L"PAUSED", 0xffffff, selectionH_, false);
}

