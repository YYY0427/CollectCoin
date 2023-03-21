#include "Credit.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../InputState.h"
#include "../DrawFunctions.h"
#include "../game.h"
#include <DxLib.h>

namespace
{
	constexpr int pw_width = Game::SCREEN_WIDTH;	// 画面の幅
	constexpr int pw_height = Game::SCREEN_HEIGHT;	// 画面の高さ
	constexpr int pw_start_x = 0;					// 画面の左
	constexpr int pw_start_y = 0;					// 画面の枠上

	constexpr int CB_WIDTH_1 = 700;												// オプション枠の幅
	constexpr int CB_HEIGHT_1 = 500;											// オプション枠の高さ
	constexpr int CB_START_X_1 = (Game::SCREEN_WIDTH / 2) - (CB_WIDTH_1 / 2);	// オプション枠の左
	constexpr int CB_START_Y_1 = (Game::SCREEN_HEIGHT / 2) - (CB_HEIGHT_1 / 2);	// オプション枠上

	constexpr int nameCount = 2;

	// オプション
	constexpr const char* const CREDIT = "CREDIT";
}

Credit::Credit(SceneManager& manager) :
	Scene(manager)
{
	controllerH_ = my::MyLoadGraph("Data/img/game/controller.png");
	teachH_ = CreateFontToHandle("PixelMplus10", 25, 0);
	creditH_ = CreateFontToHandle("PixelMplus10", 40, 0);
}

Credit::~Credit()
{
	
}

void Credit::Update(const InputState& input)
{
	// タイトル画面に戻る
	if (input.IsTriggered(InputType::prev))
	{
		manager_.PopScene();
	}
}

void Credit::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	DrawBox(CB_START_X_1 + 10, CB_START_Y_1 + 10, CB_START_X_1 + CB_WIDTH_1 + 10, CB_START_Y_1 + CB_HEIGHT_1 + 10, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, Game::SCREEN_HEIGHT - 60, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(CB_START_X_1, CB_START_Y_1, CB_START_X_1 + CB_WIDTH_1, CB_START_Y_1 + CB_HEIGHT_1, GetColor(200, 200, 200), true);

	// Music
	DrawStringToHandle(100, 100, "Music", 0x000000, teachH_, false);

	// オプション
	int width1 = GetDrawStringWidthToHandle(CREDIT, static_cast<int>(strlen(CREDIT)), creditH_);
	DrawStringToHandle((Game::SCREEN_WIDTH / 2) - (width1 / 2), CB_START_Y_1 + 50, CREDIT, 0x000000, creditH_, false);

	// B もどる
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 570, Game::SCREEN_HEIGHT - 30, 32, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawStringToHandle(Game::SCREEN_WIDTH / 2 + 600, Game::SCREEN_HEIGHT - 45, "もどる", 0xffffff, teachH_, false);

}
