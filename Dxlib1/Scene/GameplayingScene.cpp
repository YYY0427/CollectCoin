#include "GameplayingScene.h"
#include "../InputState.h"
#include "GameoverScene.h"
#include "GameclearScene.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "../DrawFunctions.h"
#include "PauseScene.h"
#include"../Game/Player.h"
#include "../Game/Field.h"
#include "../Game/Skeleton.h"
#include "../Game/Slime.h"
#include "../Game/Ghost.h"
#include "../Game/Golem.h"
#include "../Game/EnemyBase.h"
#include "../Game.h"
#include "../Game/Map.h"
#include "../Game/BackGround.h"
#include "../SoundManager.h"
#include "../Game/Coin.h"
#include "../Particle.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	// �v���C���[�̏����ʒu
	constexpr int TUTORIAL_PLAYER_START_INDEX_X = 5;
	constexpr int TUTORIAL_PLAYER_START_INDEX_Y = 5;

	constexpr int PLAYER_START_INDEX_X = 9;
	constexpr int PLAYER_START_INDEX_Y = 16;

	// �G(�X�P���g��)�̏����ʒu
	constexpr int TUTORIAL_SKELETON_START_INDEX_X = 5;
	constexpr int TUTORIAL_SKELETON_START_INDEX_Y = 1;

	constexpr int SKELETON_START_INDEX_X = 9;
	constexpr int SKELETON_START_INDEX_Y = 8;

	// �G(�X���C��)�̏����ʒu
	constexpr int SLIME_START_INDEX_X = 8;
	constexpr int SLIME_START_INDEX_Y = 10;

	// �G(�H��)�̏����ʒu
	constexpr int GHOST_START_INDEX_X = 9;
	constexpr int GHOST_START_INDEX_Y = 10;

	// �G(�S�[����)�̏����ʒu
	constexpr int GOLEM_START_INDEX_X = 10;
	constexpr int GOLEM_START_INDEX_Y = 10;

	// ������
	const char* const REDY_STRING = "REDEY";

	// �Q�[���N���A�[
	const char* const GAMECLEAR_STRING = "GAME CLEAR!";

	// �Q�[���I�[�o�[������
	const char* const GAMEOVER_STRING = "GAME OVER...";

	// �I�����̐�
	constexpr int CHOICE_NUM = 3; 

	constexpr int pw_width_1 = 400;												//�|�[�Y�g�̕�
	constexpr int pw_height_1 = 100;												//�|�[�Y�g�̍���
	constexpr int pw_start_x_1 = (Game::kScreenWidth / 2) - (pw_width_1 / 2);	//�|�[�Y�g�̍�
	constexpr int pw_start_y_1 = Game::kScreenHeight / 2;					//�|�[�Y�g��

	constexpr int pw_width_2 = 400;												//�|�[�Y�g�̕�
	constexpr int pw_height_2 = 100;												//�|�[�Y�g�̍���
	constexpr int pw_start_x_2 = (Game::kScreenWidth / 2) - (pw_width_1 / 2);	//�|�[�Y�g�̍�
	constexpr int pw_start_y_2 = Game::kScreenHeight / 2 + 115;					//�|�[�Y�g��

	constexpr int pw_width_3 = 400;												//�|�[�Y�g�̕�
	constexpr int pw_height_3 = 100;												//�|�[�Y�g�̍���
	constexpr int pw_start_x_3 = (Game::kScreenWidth / 2) - (pw_width_1 / 2);	//�|�[�Y�g�̍�
	constexpr int pw_start_y_3 = Game::kScreenHeight / 2 + 230;					//�|�[�Y�g��
}

GameplayingScene::GameplayingScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&GameplayingScene::FadeInUpdate),
	life_(3),
	timer_(0),
	gameOverTimer_(0),
	gameClearTimer_(0),
	preparTimer_(0),
	quakeTimer_(0),
	quakeX_(0.0f),
	quakeY_(0.0f),
	currentInputIndex_(0),
	isGameClear_(false),
	faideEnabled_(false),
	playerDeadSound_(false),
	isNextStage_(false),
	isTitile_(false),
	isRetry_(false),
	isCoinEnabled_(false)
{
	stage_ = tutorial;

	// ��ʕ��@��ʍ��@�r�b�g��
	int sw, sh, bit;
	// ���ƍ������擾
	GetScreenState(&sw, &sh, &bit);
	// ���H�p��ʂ�p��
	tempScreenH_ = MakeScreen(sw, sh);
	// ���Ȃ������ꍇ�����Œ�~
	assert(tempScreenH_ >= 0);

	// �t�H���g�̍쐬
	gameOverH_ = CreateFontToHandle("PixelMplus10", 50, 30);
	gameOverShadowH_ = CreateFontToHandle("PixelMplus10", 51, 30);
	gameClearH_ = CreateFontToHandle("PixelMplus10", 80, 30);
	gameClearShadowH_ = CreateFontToHandle("PixelMplus10", 81, 30);
	readyH_ = CreateFontToHandle("PixelMplus10", 20, 10);

	int stringWidth = GetDrawStringWidthToHandle(GAMECLEAR_STRING, strlen(GAMECLEAR_STRING), gameClearH_);
	int stringHeight = GetFontSizeToHandle(gameClearH_);

	gameClearPos_.x = (Game::kScreenWidth / 2) - (stringWidth / 2);
	gameClearPos_.y = (Game::kScreenHeight / 2) - 250;

	// �摜�̃��[�h
	int nowaponPlayerH = my::MyLoadGraph("Data/img/game/nowapon-player.png");
	int waponPlayerH = my::MyLoadGraph("Data/img/game/wapon-player_gold.png");
	int deadPlayerH = my::MyLoadGraph("Data/img/game/player-deth1.png");
	int attackPlayerH = my::MyLoadGraph("Data/img/game/player-attack_gold.png");

	playH_ = my::MyLoadGraph("Data/img/play.png");
	//retryH_ = my::MyLoadGraph("Data/img/setting.png");
	//doorH_ = my::MyLoadGraph("Data/img/question.png");

	skeletonH_ = my::MyLoadGraph("Data/img/game/skeleton_monokuro.png");
 	slimeH_ = my::MyLoadGraph("Data/img/game/slime_monokuro.png");
	ghostH_ = my::MyLoadGraph("Data/img/game/ghost_monokuro.png");
	golemH_ = my::MyLoadGraph("Data/img/game/golem_monokuro.png");

	int mapChipH = my::MyLoadGraph("Data/img/game/mapchip1.png");
	int backGraph = my::MyLoadGraph("Data/img/game/Gray.png");

	int sordH_ =  my::MyLoadGraph("Data/img/game/sord_gold.png");
	int doorH_ =  my::MyLoadGraph("Data/img/game/door.png");
	lifeH_ = my::MyLoadGraph("Data/img/game/hart.png");
	coinH_ = my::MyLoadGraph("Data/img/game/coin.png");

	StageCheck(stage_);

	pPlayer_ = std::make_shared<Player>(nowaponPlayerH, waponPlayerH, deadPlayerH, attackPlayerH, playerStartPosX_, playerStartPosY_, stage_);
	pField_ = std::make_shared<Field>(sordH_, doorH_, coinH_, stage_);
	pMap_ = std::make_shared<Map>(mapChipH, stage_);
	pBackGround_ = std::make_shared<BackGround>(backGraph);

	for (auto& enemy : pEnemy_)
	{
		enemy->SetPlayer(pPlayer_);
		enemy->SetField(pField_);
	}
	
	pBackGround_->SetPlayer(pPlayer_);
	pPlayer_->SetField(pField_);
	pField_->SetPlayer(pPlayer_);
	pMap_->SetField(pField_);
	pField_->StageCheck(stage_);
//	pField_->StageCheck2(stage_);

	for (int i = 0; i < enemyNum_; i++)
	{
		pPlayer_->SetEnemy(pEnemy_[i], i, stage_);
		pField_->SetEnemy(pEnemy_[i], i, stage_);
	}

	SoundManager::GetInstance().PlayMusic("Data/sound/BGM/game.mp3");
	SetVolumeMusic(0);
}

GameplayingScene::~GameplayingScene()
{
	
}

void GameplayingScene::FadeInUpdate(const InputState& input)
{
	for (auto& enemy : pEnemy_)
	{
		enemy->SetEnabled(false);
	}

	pPlayer_->SetEnabled(false);
	SetVolumeMusic(static_cast<int>(255.0f / 60.0f * static_cast<float>(60 - fadeTimer_)) * (static_cast<float>(SoundManager::GetInstance().GetBGMVolume() / 255.0f)));
	fadeValue_ = 255 * (static_cast<int>(fadeTimer_)) / static_cast<int>(fade_interval);
	if (--fadeTimer_ == 0)
	{
		faideEnabled_ = true;

		fadeValue_ = 0;

		preparTimer_ = 120;

		updateFunc_ = &GameplayingScene::PrepareUpdate;
	}
}

void GameplayingScene::NormalUpdate(const InputState& input)
{	
	if (faideEnabled_)
	{
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(true);
		}

		pPlayer_->SetEnabled(true);

		faideEnabled_ = false;
	}

	pBackGround_->Update();

	pField_->Updata();

	pPlayer_->Update(input);

	for (auto& enemy : pEnemy_)
	{
		enemy->Update();
	}

	for (auto& enemy : pEnemy_)
	{
		// �v���C���[�ƓG�̓����蔻��
		if (Colision(enemy, enemy->GetSizeX(), enemy->GetSizeY()))
		{
			// �v���C���[���C�W�P��Ԃł͂Ȃ��Ƃ��ɓG�Ɠ��������ꍇ
			if (!enemy->GetIzike())
			{
				// BGM���~�߂�
				StopMusic();
				pPlayer_->StopMusic();

				// �c�@ - 1
				life_--;

				// �Q�[���I�[�o�[�̏ꍇ��ʂ̗h���傫������
				if (life_ <= 0)
				{
					quakeX_ = 40.0f;
					quakeY_ = 20.0f;
					quakeTimer_ = 80;
				}
				else
				{
					quakeX_ = 20.0f;
					quakeTimer_ = 60;
				}

				// �v���C���[�̎��S�t���O�𗧂Ă�
				pPlayer_->SetDead(true);

				fadeColor_ = 0xff0000;

				SoundManager::GetInstance().Play("enemyAttack");

				playerDeadSound_ = true;

				// �c�@���Ȃ������ꍇ
				if (life_ <= 0)
				{
					updateFunc_ = &GameplayingScene::GameOverUpdate;
				}
				else if (life_ > 0)
				{
					// �v���C���[���S���o�ڍs
					updateFunc_ = &GameplayingScene::PlayerDeadUpdate;
				}
			}
			// �G���C�W�P��Ԃ̏ꍇ�ɓG�Ɠ��������ꍇ�G���E��
			else
			{
				// �G�̎��S�t���O�𗧂Ă�
				enemy->SetDead(true);

				SoundManager::GetInstance().Play("kill");

				updateFunc_ = &GameplayingScene::EnemyDeadUpdate;
			}
		}
	}

	if (quakeTimer_ > 0)
	{
		quakeX_ = -quakeX_;
		quakeX_ *= 0.95f;
		quakeY_ = -quakeY_;
		quakeY_ *= 0.95f;

		--quakeTimer_;
	}
	else
	{
		quakeX_ = 0.0f;
		quakeY_ = 0.0f;
	}

	// �|�[�Y�V�[���؂�ւ�
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PushScene(new PauseScene(manager_));
	}

	// �Q�[���N���A����
	if (pField_->IsGameClearCheck())
	{
		// BGM���~�߂�
		StopMusic();
		pPlayer_->StopMusic();

		// �Q�[���N���A���o�Ɉڍs
		updateFunc_ = &GameplayingScene::GameClearUpdate;
	}
}

void GameplayingScene::EnemyDeadUpdate(const InputState& input)
{
	pPlayer_->SetKill(true);

	pPlayer_->EnemyKillUpdate();

	if (pPlayer_->GetAnimeEnd())
	{
		pPlayer_->SetAnimeEnd(false);
		pPlayer_->SetKill(false);
		pPlayer_->SetAttackIdx(0);

		updateFunc_ = &GameplayingScene::NormalUpdate;
	}
}

void GameplayingScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void GameplayingScene::Draw()
{
	// ���H�p�X�N���[���n���h�����Z�b�g
	SetDrawScreen(tempScreenH_);

	// �w�i�̕`��
	pBackGround_->Draw();

	// �}�b�v�̕`��
	pMap_->Draw();

	// �t�B�[���h(���A�R�C��)�̕`��
	pField_->Draw();

	// �G�̕`��
	for (auto& enemy : pEnemy_)
	{
		enemy->Draw();
	}

	// �v���C���[�̕`��
	pPlayer_->Draw();

	if (isCoinEnabled_)
	{
		for (auto& coin : pCoin_)
		{
			coin->Draw();
		}
	}

	// �c�@�̕`��
	for (int i = 0; i < life_; i++)
	{
		int x = Game::kScreenWidth / 2 + 250 + (-i * 40);

		DrawRectRotaGraph(x, Game::kScreenHeight - 30, 0, 0, 16, 16, 2.5f, 0.0f, lifeH_, true);
	}


	// �Q�[���I�[�o�[
	if (isGameOver_)
	{
		int stringWidth = GetDrawStringWidthToHandle(GAMEOVER_STRING, strlen(GAMEOVER_STRING), gameOverH_);
		int stringHeight = GetFontSizeToHandle(gameOverH_);

		int width = (Game::kScreenWidth / 2) - (stringWidth / 2);
		int height = (Game::kScreenHeight / 2) - (stringHeight / 2);

		// �Q�[���I�[�o�[�����̕\��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, gameOverFadeValue_);
		DrawStringToHandle(width - 3, height,
			GAMEOVER_STRING, 0x000000, gameOverShadowH_, false);
		DrawStringToHandle(width, height,
			GAMEOVER_STRING, 0xffffff, gameOverH_, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	// �Q�[���N���A
	if (isGameClear_ /*&& gameClearTimer_ >= 120*/)
	{
		/*int stringWidth = GetDrawStringWidthToHandle(GAMECLEAR_STRING, strlen(GAMECLEAR_STRING), gameClearH_);
		int stringHeight = GetFontSizeToHandle(gameClearH_);

		int width = (Game::kScreenWidth / 2) - (stringWidth / 2);
		int height = (Game::kScreenHeight / 2) - 300;*/

		// �Q�[���N���A�����̕\��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, gameClearStringFadeValue_);
		DrawStringToHandle(gameClearPos_.x - 3, gameClearPos_.y,
			GAMECLEAR_STRING, 0x000000, gameClearShadowH_, false);
		DrawStringToHandle(gameClearPos_.x, gameClearPos_.y,
			GAMECLEAR_STRING, 0xffffff, gameClearH_, false);
		if (stage_ != 1)
		{
			// �I����
			DrawRoundRect(pw_start_x_1 - 3, pw_start_y_1 - 3, pw_start_x_1 + pw_width_1 + 3, pw_start_y_1 + pw_height_1 + 3, 5, 5, 0x000000, true);
			DrawRoundRect(pw_start_x_2 - 3, pw_start_y_2 - 3, pw_start_x_2 + pw_width_2 + 3, pw_start_y_2 + pw_height_2 + 3, 5, 5, 0x000000, true);
			DrawRoundRect(pw_start_x_3 - 3, pw_start_y_3 - 3, pw_start_x_3 + pw_width_3 + 3, pw_start_y_3 + pw_height_3 + 3, 5, 5, 0x000000, true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawRoundRect(pw_start_x_1, pw_start_y_1, pw_start_x_1 + pw_width_1, pw_start_y_1 + pw_height_1, 5, 5, 0xffffff, true);
			DrawRoundRect(pw_start_x_2, pw_start_y_2, pw_start_x_2 + pw_width_2, pw_start_y_2 + pw_height_2, 5, 5, 0xffffff, true);
			DrawRoundRect(pw_start_x_3, pw_start_y_3, pw_start_x_3 + pw_width_3, pw_start_y_3 + pw_height_3, 5, 5, 0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawRoundRect(pw_start_x_1, pw_start_y_1, pw_start_x_1 + pw_width_1, pw_start_y_1 + pw_height_1 - 5, 5, 5, 0xffffff, true);
			DrawRoundRect(pw_start_x_2, pw_start_y_2, pw_start_x_2 + pw_width_2, pw_start_y_2 + pw_height_2 - 5, 5, 5, 0xffffff, true);
			DrawRoundRect(pw_start_x_3, pw_start_y_3, pw_start_x_3 + pw_width_3, pw_start_y_3 + pw_height_3 - 5, 5, 5, 0xffffff, true);

			// �A�C�R��
		//	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2 + 150, 1.0f, 0.0f, playH_, true);
		//	DrawRotaGraph(Game::kScreenWidth / 2 - 140, Game::kScreenHeight / 2 + 265, 1.0f, 0.0f, doorH_, true);
		//	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2 + 265, 1.0f, 0.0f, retryH_, true);

			DrawFormatString(0, 0, 0xffffff, "currentInputIndex = %d", currentInputIndex_);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (preparTimer_ > 0)
	{
		int width = GetDrawStringWidthToHandle(REDY_STRING, strlen(REDY_STRING), readyH_);
		// �����������̕\��
		DrawStringToHandle((Game::kScreenWidth / 2) - (width / 2), Game::kScreenHeight / 2 + 40,
			REDY_STRING, 0xffffff, readyH_, false);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(quakeX_, quakeY_, tempScreenH_, false);
	if (quakeTimer_ > 0)
	{
		GraphFilter(tempScreenH_, DX_GRAPH_FILTER_MONO, 0, 7);
		DrawGraph(quakeX_, quakeY_, tempScreenH_, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void GameplayingScene::GameClearUpdate(const InputState& input)
{
	isCoinEnabled_ = true;
	if (gameClearTimer_ % 1 == 0)
	{
		pCoin_.push_back(std::make_shared<Coin>(coinH_));
		pCoin_.push_back(std::make_shared<Coin>(coinH_));
	}

	pPlayer_->ClearUpdate();
	
	for (auto& coin : pCoin_)
	{
		coin->Update();
	}

	gameClearTimer_++;
	if (gameClearTimer_ >= 60 && !isGameClear_)
	{
		SoundManager::GetInstance().PlayJingle("Data/sound/BGM/gameClear.ogg");
		isGameClear_ = true;
	//	gameClearStringFadeValue_ = 0;
	}
	

	if (isGameClear_)
	{
		gameClearStringFadeValue_ = 255 * (static_cast<float>(gameClearFadeTimer_)) / static_cast<float>(game_clear_fade_interval);

		gameClearPos_.y--;
		if (gameClearPos_.y <= 240)
		{
			gameClearPos_.y = 240;
		}

	//	gameClearFadeTimer_ = (std::min)(gameClearFadeTimer_++, 255);

		if (++gameClearFadeTimer_ >= 255)
		{
			gameClearStringFadeValue_ = 255;

			if (stage_ == 1)
			{
				isTitile_ = true;
				updateFunc_ = &GameplayingScene::FadeOutUpdate;
			}

			//�㉺�ŉ�鏈��
			if (input.IsTriggered(InputType::up))
			{
				SoundManager::GetInstance().Play("cursor");
				currentInputIndex_ = ((currentInputIndex_ - 1) + CHOICE_NUM) % CHOICE_NUM;
			}
			else if (input.IsTriggered(InputType::down))
			{
				SoundManager::GetInstance().Play("cursor");
				currentInputIndex_ = (currentInputIndex_ + 1) % CHOICE_NUM;
			}

			if (input.IsTriggered(InputType::next))
			{
				SoundManager::GetInstance().Play("decision");
				gameClearTimer_ = 0;
				if (currentInputIndex_ == 0)
				{
					isNextStage_ = true;
				}
				else if (currentInputIndex_ == 1)
				{
					isRetry_ = true;
				}
				else 
				{
					isTitile_ = true;
				}
				updateFunc_ = &GameplayingScene::FadeOutUpdate;
			}
		}
	}
}

void GameplayingScene::GameOverUpdate(const InputState& input)
{
	if (quakeTimer_ > 0)
	{
		quakeX_ = -quakeX_;
		quakeX_ *= 0.95f;
		quakeY_ = -quakeY_;
		quakeY_ *= 0.95f;

		--quakeTimer_;
	}
	else
	{
		quakeX_ = 0.0f;
		quakeY_ = 0.0f;
	}

	if (timer_++ > 120)
	{
		// ���ׂĂ̓G������
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(false);
		}

		if (!pPlayer_->GetAnimeEnd())
		{
			pPlayer_->DeadUpdate();

			if (playerDeadSound_)
			{
				SoundManager::GetInstance().PlayJingle("Data/sound/BGM/gameOver.wav");
				playerDeadSound_ = false;
			}
		}
		else if (pPlayer_->GetAnimeEnd() && !CheckMusic())
		{
			isGameOver_ = true;
		}
		if (isGameOver_)
		{
			gameOverFadeValue_ = 255 * (static_cast<float>(gameOverFadeTimer_)) / static_cast<float>(game_over_fade_interval);
			if (++gameOverFadeTimer_ >= 255)
			{
				gameOverFadeValue_ = 255;

				if (gameOverTimer_++ % 180 == 0)
				{
					updateFunc_ = &GameplayingScene::FadeOutUpdate;
				}
			}
		}
	}
}

void GameplayingScene::PlayerDeadUpdate(const InputState& input)
{
	if (quakeTimer_ > 0)
	{
		quakeX_ = -quakeX_;
		quakeX_ *= 0.95f;
		--quakeTimer_;
	}
	else
	{
		quakeX_ = 0.0f;
	}

	if (timer_++ > 60)
	{
		// ���ׂĂ̓G������
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(false);
		}
		
		if (playerDeadSound_)
		{
			SoundManager::GetInstance().PlayJingle("Data/sound/BGM/playerDead.wav");
			playerDeadSound_ = false;
		}

		if (!pPlayer_->GetAnimeEnd())
		{
			pPlayer_->DeadUpdate();
		}
	}

	if (pPlayer_->GetAnimeEnd() && !CheckMusic())
	{
		updateFunc_ = &GameplayingScene::FadeOutUpdate;
	}
}

void GameplayingScene::FadeOutUpdate(const InputState& input)
{
	fadeTimer_++;
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	SetVolumeMusic(static_cast<float>((std::max)(SoundManager::GetInstance().GetBGMVolume() - fadeValue_, 0)));

	// �Q�[���I�[�o�[
	if (fadeTimer_ > fade_interval && isGameOver_)
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
	// ���̃X�e�[�W��
	else if (fadeTimer_ > fade_interval && isNextStage_)
	{
		isNextStage_ = false;
		stage_ += 1;
		SetInit();
		updateFunc_ = &GameplayingScene::FadeInUpdate;
		return;
	}
	// �^�C�g����ʂɖ߂�
	else if (fadeTimer_ > fade_interval && isTitile_)
	{
		isTitile_ = false;
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
	// ���g���C
	else if (fadeTimer_ > fade_interval && isRetry_)
	{
		SetInit();
		updateFunc_ = &GameplayingScene::FadeInUpdate;
	}
	// �c�@���P������
	else if (fadeTimer_ > fade_interval && life_ > 0)
	{
		SetDeadInit();
		updateFunc_ = &GameplayingScene::FadeInUpdate;
	}
}

void GameplayingScene::PrepareUpdate(const InputState& input)
{
	preparTimer_--;
	if (preparTimer_ <= 0)
	{
		updateFunc_ = &GameplayingScene::NormalUpdate;
		preparTimer_ = 0;
	}
}

void GameplayingScene::SetInit()
{
	pCoin_.clear();
	fadeTimer_ = fade_interval;
	fadeValue_ = 255;
	timer_ = 0;
	isTitile_ = false;
	isCoinEnabled_ = false;
	isNextStage_ = false;
	isRetry_ = false;
	isGameClear_ = false;
	isGameOver_ = false;
	pPlayer_->Init(stage_);
	StageCheck(stage_);
	pField_->SetCoinNum(0);
//	pField_->Init();
	for (auto& enemy : pEnemy_)
	{
		enemy->Init();
		enemy->SetInit(stage_);
	}
	for (auto& enemy : pEnemy_)
	{
		enemy->SetPlayer(pPlayer_);
		enemy->SetField(pField_);
	}
	pBackGround_->SetPlayer(pPlayer_);
	pPlayer_->SetField(pField_);
	pField_->SetPlayer(pPlayer_);
	pMap_->SetField(pField_);
	pMap_->StageCheck(stage_);
	pField_->StageCheck(stage_);

	for (int i = 0; i < enemyNum_; i++)
	{
		pPlayer_->SetEnemy(pEnemy_[i], i, stage_);
		pField_->SetEnemy(pEnemy_[i], i, stage_);
	}
	SetVolumeMusic(0);
	SoundManager::GetInstance().PlayMusic("Data/sound/BGM/game.mp3");
	pPlayer_->StartMusic();
}

void GameplayingScene::SetDeadInit()
{
	fadeTimer_ = fade_interval;
	fadeValue_ = 255;
	timer_ = 0;
	isTitile_ = false;
	isCoinEnabled_ = false;
	isNextStage_ = false;
	isRetry_ = false;
	isGameClear_ = false;
	isGameOver_ = false;
	pPlayer_->Init(stage_);
	for (auto& enemy : pEnemy_)
	{
		enemy->Init();
		enemy->SetInit(stage_);
	}
}

bool GameplayingScene::Colision(std::shared_ptr<EnemyBase>enemy, int width, int height)
{
	float playerLeft = pPlayer_->GetPos().x;
	float playerRight = pPlayer_->GetPos().x + width;
	float playerTop = pPlayer_->GetPos().y;
	float playerBottom = pPlayer_->GetPos().y + width;

	float enemyLeft = enemy->GetPos().x;
	float enemyRight = enemy->GetPos().x + height;
	float enemyTop = enemy->GetPos().y;
	float enemyBottom = enemy->GetPos().y + height;

	if (playerLeft > enemyRight)	return false;
	if (playerRight < enemyLeft)	return false;
	if (playerTop > enemyBottom)	return false;
	if (playerBottom < enemyTop)	return false;

	return true;
}

void GameplayingScene::StageCheck(int stage)
{
	switch (stage)
	{
	case 0:
		playerStartPosX_ = TUTORIAL_PLAYER_START_INDEX_X;
		playerStartPosY_ = TUTORIAL_PLAYER_START_INDEX_Y;
		skeletonStartPosX_ = TUTORIAL_SKELETON_START_INDEX_X;
		skeletonStartPosY_ = TUTORIAL_SKELETON_START_INDEX_Y;
		break;
	case 1:
		playerStartPosX_ = PLAYER_START_INDEX_X;
		playerStartPosY_ = PLAYER_START_INDEX_Y;
		skeletonStartPosX_ = SKELETON_START_INDEX_X;
		skeletonStartPosY_ = SKELETON_START_INDEX_Y;
		slimeStartPosX_ = SLIME_START_INDEX_X;
		slimeStartPosY_ = SLIME_START_INDEX_Y;
		ghostStartPosX_ = GHOST_START_INDEX_X;
		ghostStartPosY_ = GHOST_START_INDEX_Y;
		golemStartPosX_ = GOLEM_START_INDEX_X;
		golemStartPosY_ = GOLEM_START_INDEX_Y;
		break;
	default:
		break;
	}

	if (stage >= 1)
	{
		enemyNum_ = EnemyBase::enemy_num;
		pEnemy_.resize(enemyNum_);
		pEnemy_[EnemyBase::skeleton] = std::make_shared<Skeleton>(
			skeletonH_,
			skeletonStartPosX_,
			skeletonStartPosY_,
			stage_);
		pEnemy_[EnemyBase::slime] = std::make_shared<Slime>(
			slimeH_,
			slimeStartPosX_,
			slimeStartPosY_,
			stage_);
		pEnemy_[EnemyBase::ghost] = std::make_shared<Ghost>(
			ghostH_,
			ghostStartPosX_,
			ghostStartPosY_,
			stage_);
		pEnemy_[EnemyBase::golem] = std::make_shared<Golem>(
			golemH_,
			golemStartPosX_,
			golemStartPosY_,
			stage_);
	}
	else
	{
		enemyNum_ = 1;
		pEnemy_.resize(enemyNum_);
		pEnemy_[EnemyBase::skeleton] = std::make_shared<Skeleton>(
			skeletonH_,					// �摜�n���h��
			skeletonStartPosX_,			// �������WX
			skeletonStartPosY_,			// �������WY 
			stage_);
	}
}
