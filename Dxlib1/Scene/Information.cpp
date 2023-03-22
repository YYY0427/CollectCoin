#include "Information.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../InputState.h"
#include "../DrawFunctions.h"
#include "../game.h"
#include <DxLib.h>

namespace
{
	constexpr int WS_WIDTH = Game::SCREEN_WIDTH;	// 画面の幅
	constexpr int WS_HEIGHT = Game::SCREEN_HEIGHT;	// 画面の高さ
	constexpr int WS_START_X = 0;					// 画面の左
	constexpr int WS_START_Y = 0;					// 画面の枠上

	constexpr int CD_WIDTH_1 = 700;												// 枠の幅
	constexpr int CD_HEIGHT_1 = 550;											// 枠の高さ
	constexpr int CD_START_X_1 = (Game::SCREEN_WIDTH / 2) - (CD_WIDTH_1 / 2);	// 枠の左
	constexpr int CD_START_Y_1 = (Game::SCREEN_HEIGHT / 2) - (CD_HEIGHT_1 / 2);	// 枠上

	// 選択肢の数
	constexpr int NAME_COUNT = 2;

	// インフォメーション
	constexpr const char* const CREDIT = "Information";
}

Information::Information(SceneManager& manager) :
	Scene(manager),
	controllerH_(-1),
	teachH_(-1),
	titleH_(-1)
{
	Init();
}

Information::~Information()
{
	End();
}

void Information::Init()
{
	// 画像のロード
	controllerH_ = my::MyLoadGraph("Data/img/game/controller.png");

	// フォントの作成
	teachH_ = CreateFontToHandle("PixelMplus10", 30, 0);
	titleH_ = CreateFontToHandle("PixelMplus10", 40, 0);
}

void Information::End()
{
	DeleteGraph(controllerH_);
	DeleteFontToHandle(teachH_);
	DeleteFontToHandle(titleH_);
}

void Information::Update(const InputState& input)
{
	// タイトル画面に戻る
	if (input.IsTriggered(InputType::prev))
	{
		manager_.PopScene();
	}
}

void Information::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(WS_START_X, WS_START_Y, WS_START_X + WS_WIDTH, WS_START_Y + WS_HEIGHT, 0x000000, true);
	DrawBox(CD_START_X_1 + 10, CD_START_Y_1 + 10, CD_START_X_1 + CD_WIDTH_1 + 10, CD_START_Y_1 + CD_HEIGHT_1 + 10, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, Game::SCREEN_HEIGHT - 60, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(CD_START_X_1, CD_START_Y_1, CD_START_X_1 + CD_WIDTH_1, CD_START_Y_1 + CD_HEIGHT_1, GetColor(200, 200, 200), true);

	// 開発者
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 100, "PROGRAMMER", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 100, "米満　悠人", 0x004c71, teachH_, false);

	// 開発環境
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 150, "DEV", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 150, "Visual Studio 2022", 0x004c71, teachH_, false);

	// 使用ライブラリ
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 200, "LIBRARY", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 200, "DXライブラリ", 0x004c71, teachH_, false);

	// 使用言語
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 250, "LAUNGUAGE", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 250, "C++", 0x004c71, teachH_, false);

	// 使用ツール
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 300, "TOOL", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 300, "Adobe Photoshop 2022", 0x004c71, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 335, "Platinum", 0x004c71, teachH_, false);

	// 参考ゲーム
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 385, "REFERENCE", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 385, "PAC-MAN", 0x004c71, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 420, "ムーブロック", 0x004c71, teachH_, false);

	// 期間
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 470, "PERIOD", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 470, "3 MONTH", 0x004c71, teachH_, false);

	// オプション
	int width1 = GetDrawStringWidthToHandle(CREDIT, static_cast<int>(strlen(CREDIT)), titleH_);
	DrawStringToHandle((Game::SCREEN_WIDTH / 2) - (width1 / 2), CD_START_Y_1 + 30, CREDIT, 0x000000, titleH_, false);

	// B もどる
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 570, Game::SCREEN_HEIGHT - 30, 32, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawStringToHandle(Game::SCREEN_WIDTH / 2 + 600, Game::SCREEN_HEIGHT - 45, "もどる", 0xffffff, teachH_, false);

}
