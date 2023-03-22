#include "Information.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../InputState.h"
#include "../DrawFunctions.h"
#include "../game.h"
#include <DxLib.h>

namespace
{
	constexpr int WS_WIDTH = Game::SCREEN_WIDTH;	// ��ʂ̕�
	constexpr int WS_HEIGHT = Game::SCREEN_HEIGHT;	// ��ʂ̍���
	constexpr int WS_START_X = 0;					// ��ʂ̍�
	constexpr int WS_START_Y = 0;					// ��ʂ̘g��

	constexpr int CD_WIDTH_1 = 700;												// �g�̕�
	constexpr int CD_HEIGHT_1 = 550;											// �g�̍���
	constexpr int CD_START_X_1 = (Game::SCREEN_WIDTH / 2) - (CD_WIDTH_1 / 2);	// �g�̍�
	constexpr int CD_START_Y_1 = (Game::SCREEN_HEIGHT / 2) - (CD_HEIGHT_1 / 2);	// �g��

	// �I�����̐�
	constexpr int NAME_COUNT = 2;

	// �C���t�H���[�V����
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
	// �摜�̃��[�h
	controllerH_ = my::MyLoadGraph("Data/img/game/controller.png");

	// �t�H���g�̍쐬
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
	// �^�C�g����ʂɖ߂�
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

	// �J����
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 100, "PROGRAMMER", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 100, "�Ė��@�I�l", 0x004c71, teachH_, false);

	// �J����
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 150, "DEV", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 150, "Visual Studio 2022", 0x004c71, teachH_, false);

	// �g�p���C�u����
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 200, "LIBRARY", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 200, "DX���C�u����", 0x004c71, teachH_, false);

	// �g�p����
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 250, "LAUNGUAGE", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 250, "C++", 0x004c71, teachH_, false);

	// �g�p�c�[��
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 300, "TOOL", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 300, "Adobe Photoshop 2022", 0x004c71, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 335, "Platinum", 0x004c71, teachH_, false);

	// �Q�l�Q�[��
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 385, "REFERENCE", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 385, "PAC-MAN", 0x004c71, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 420, "���[�u���b�N", 0x004c71, teachH_, false);

	// ����
	DrawStringToHandle(CD_START_X_1 + 100, CD_START_Y_1 + 470, "PERIOD", 0x000000, teachH_, false);
	DrawStringToHandle(CD_START_X_1 + 300, CD_START_Y_1 + 470, "3 MONTH", 0x004c71, teachH_, false);

	// �I�v�V����
	int width1 = GetDrawStringWidthToHandle(CREDIT, static_cast<int>(strlen(CREDIT)), titleH_);
	DrawStringToHandle((Game::SCREEN_WIDTH / 2) - (width1 / 2), CD_START_Y_1 + 30, CREDIT, 0x000000, titleH_, false);

	// B ���ǂ�
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 570, Game::SCREEN_HEIGHT - 30, 32, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawStringToHandle(Game::SCREEN_WIDTH / 2 + 600, Game::SCREEN_HEIGHT - 45, "���ǂ�", 0xffffff, teachH_, false);

}
