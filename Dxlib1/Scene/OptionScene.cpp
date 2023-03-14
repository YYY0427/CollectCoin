#include "OptionScene.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../game.h"
#include "../DrawFunctions.h"
#include <DxLib.h>

OptionScene::OptionScene(SceneManager& manager):
	Scene(manager)
{
	handle_ = my::MyLoadGraph("Data/img/game/controller.png");
	teachH_ = CreateFontToHandle("PixelMplus10", 25, 0);
}

void OptionScene::Update(const InputState& input)
{
	if (input.IsTriggered(InputType::prev))
	{
		manager_.PopScene();
		return;
	}
}

void OptionScene::Draw()
{
	constexpr int pw_width = Game::kScreenWidth;	// �|�[�Y�g�̕�
	constexpr int pw_height = Game::kScreenHeight;	// �|�[�Y�g�̍���
	constexpr int pw_start_x = 0;					// �|�[�Y�g�̍�
	constexpr int pw_start_y = 0;					// �|�[�Y�g��
	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	constexpr int pw_width_1 = 700;										//�|�[�Y�g�̕�
	constexpr int pw_height_1 = 500;									//�|�[�Y�g�̍���
	constexpr int pw_start_x_1 = (Game::kScreenWidth / 2) - (pw_width_1 / 2);	//�|�[�Y�g�̍�
	constexpr int pw_start_y_1 = (Game::kScreenHeight / 2) - (pw_height_1 / 2);	//�|�[�Y�g��
	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	DrawBox(pw_start_x_1 + 10, pw_start_y_1 + 10, pw_start_x_1 + pw_width_1 + 10, pw_start_y_1 + pw_height_1 + 10, 0x000000, true);
	DrawBox(0, Game::kScreenHeight - 60, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(pw_start_x_1, pw_start_y_1, pw_start_x_1 + pw_width_1, pw_start_y_1 + pw_height_1, GetColor(200, 200, 200), true);

	constexpr const char* const OPTION = "OPTION";
	int width1 = GetDrawStringWidthToHandle(OPTION, strlen(OPTION), teachH_);
	DrawStringToHandle((Game::kScreenWidth / 2) - (width1 / 2), pw_start_y_1 + 50, OPTION, 0x000000, teachH_, false);

	// A �����Ă�
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 570, Game::kScreenHeight - 30, 0, 0, 16, 16, 2.0f, 0.0f, handle_, true);
	DrawStringToHandle(Game::kScreenWidth / 2 + 600, Game::kScreenHeight - 45, "�����Ă�", 0xffffff, teachH_, false);

	// B ���ǂ�
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 80, Game::kScreenHeight - 30, 32, 0, 16, 16, 2.0f, 0.0f, handle_, true);
	DrawStringToHandle(Game::kScreenWidth / 2 + 110, Game::kScreenHeight - 45, "���ǂ�", 0xffffff, teachH_, false);

	// ���������@���񂽂�
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 250, Game::kScreenHeight - 30, 128, 0, 16, 16, 2.0f, 0.0f, handle_, true);
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 255 + 32, Game::kScreenHeight - 30, 144, 0, 16, 16, 2.0f, 0.0f, handle_, true);
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 260 + 64, Game::kScreenHeight - 30, 160, 0, 16, 16, 2.0f, 0.0f, handle_, true);
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 265 + 96, Game::kScreenHeight - 30, 176, 0, 16, 16, 2.0f, 0.0f, handle_, true);
	DrawStringToHandle(Game::kScreenWidth / 2 + 270 + 128, Game::kScreenHeight - 45, "���񂽂�", 0xffffff, teachH_, false);
}