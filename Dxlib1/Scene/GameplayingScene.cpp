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
#include <DxLib.h>

namespace
{
	// �v���C���[�̏����ʒu
	constexpr int PLAYER_START_INDEX_X = 9;
	constexpr int PLAYER_START_INDEX_Y = 16;

	// �G(�X�P���g��)�̏����ʒu
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
	const char* const GAMECLEAR_STRING = "GAME CLEAR!!!";

	// �Q�[���I�[�o�[������
	const char* const GAMEOVER_STRING = "GAME OVER!!!";
}

GameplayingScene::GameplayingScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&GameplayingScene::FadeInUpdate),
	life_(1),
	timer_(0),
	gameOverTimer_(0),
	gameClearTimer_(0),
	preparTimer_(0),
	isGameClear_(false),
	faideEnabled_(false)
{
	// �t�H���g�̍쐬
	gameOverH_ = CreateFontToHandle("PixelMplus10", 20, 10);
	gameClearH_ = CreateFontToHandle("PixelMplus10", 20, 10);
	readyH_ = CreateFontToHandle("PixelMplus10", 20, 10);

	// �T�E���h�̃��[�h
	int coinSoundH = my::MyLoadSound("Data/sound/coin.wav");
	int sordSoundH = my::MyLoadSound("Data/sound/sord.wav");
	int powerDownSoundH = my::MyLoadSound("Data/sound/powerDown.wav");
	killSoundH_ = my::MyLoadSound("Data/sound/kill.wav");
	enemyAttackSoundH_ = my::MyLoadSound("Data/sound/enemyAttack.wav");

	// �摜�̃��[�h
	int nowaponPlayerH = my::MyLoadGraph("Data/img/game/nowapon-player.png");
	int waponPlayerH = my::MyLoadGraph("Data/img/game/wapon-player.png");
	int deadPlayerH = my::MyLoadGraph("Data/img/game/player-deth.png");
	int attackPlayerH = my::MyLoadGraph("Data/img/game/player-attack.png");

	int skeletonH = my::MyLoadGraph("Data/img/game/skeleton_walk.png");
 	int slimeH = my::MyLoadGraph("Data/img/game/slime.png");
	int ghostH = my::MyLoadGraph("Data/img/game/whiteGhost.png");
	int golemH = my::MyLoadGraph("Data/img/game/golem.png");

	int mapChipH = my::MyLoadGraph("Data/img/game/mapchip.png");
	int backGraph = my::MyLoadGraph("Data/img/game/Gray.png");
	lifeH_ = my::MyLoadGraph("Data/img/game/hart.png");


	pEnemy_[EnemyBase::skeleton] = std::make_shared<Skeleton>(
								skeletonH,					// �摜�n���h��
								SKELETON_START_INDEX_X,		// �������WX
								SKELETON_START_INDEX_Y);	// �������WY
	pEnemy_[EnemyBase::slime] = std::make_shared<Slime>(
								slimeH,						// �摜�n���h��
								SLIME_START_INDEX_X,		// �������WX
								SLIME_START_INDEX_Y);		// �������WY
	pEnemy_[EnemyBase::ghost] = std::make_shared<Ghost>(
								ghostH,						// �摜�n���h��
								GHOST_START_INDEX_X,		// �������WX
								GHOST_START_INDEX_Y);		// �������WY
	pEnemy_[EnemyBase::golem] = std::make_shared<Golem>(
								golemH,						// �摜�n���h��
								GOLEM_START_INDEX_X,		// �������WX
								GOLEM_START_INDEX_Y);		// �������WY

	pPlayer_ = std::make_shared<Player>(nowaponPlayerH, waponPlayerH, deadPlayerH, attackPlayerH,
										powerDownSoundH, PLAYER_START_INDEX_X, PLAYER_START_INDEX_Y);
	pField_ = std::make_shared<Field>(coinSoundH, sordSoundH);
	pMap_ = std::make_shared<Map>(mapChipH);
	pBackGround_ = std::make_shared<BackGround>(backGraph);

	for (auto& enemy : pEnemy_)
	{
		enemy->SetPlayer(pPlayer_);
		enemy->SetField(pField_);
	}
	
	pPlayer_->SetField(pField_);
	pField_->SetPlayer(pPlayer_);

	for (int i = 0; i < EnemyBase::enemy_num; i++)
	{
		pPlayer_->SetEnemy(pEnemy_[i], i);
		pField_->SetEnemy(pEnemy_[i], i);
	}
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

	fadeValue_ = 255 * (static_cast<float>(fadeTimer_)) / static_cast<float>(fade_interval);
	ChangeVolumeSoundMem(255 - fadeValue_, bgmSoundH_);
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
			// �G���C�W�P��Ԃł͂Ȃ��Ƃ��ɓG�Ɠ��������ꍇ
			if (!enemy->GetIzike())
			{
				// �c�@ - 1
				life_--;

				// �v���C���[�̎��S�t���O�𗧂Ă�
				pPlayer_->SetDead(true);

				//// ���ׂĂ̓G������
				//for (auto& enemy : pEnemy_)
				//{
				//	enemy->SetEnabled(false);
				//}

				fadeColor_ = 0xff0000;

				PlaySoundMem(enemyAttackSoundH_, DX_PLAYTYPE_BACK);

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

				PlaySoundMem(killSoundH_, DX_PLAYTYPE_BACK);

				updateFunc_ = &GameplayingScene::EnemyDeadUpdate;
			}
		}
	}

	// �|�[�Y�V�[���؂�ւ�
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PushScene(new PauseScene(manager_));
	}

	// �Q�[���N���A����
	if (pField_->IsGameClearCheck())
	{
		// �Q�[���N���A���o�Ɉڍs
		updateFunc_ = &GameplayingScene::GameClearUpdate;
		fadeColor_ = 0xff0000;
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

	// �Q�[���I�[�o�[
	if (isGameOver_)
	{
		int width = GetDrawStringWidthToHandle(GAMEOVER_STRING, strlen(GAMEOVER_STRING), gameOverH_);

		// �Q�[���I�[�o�[�����̕\��
		DrawStringToHandle((Game::kScreenWidth / 2) - (width / 2), Game::kScreenHeight / 2 + 40,
			GAMEOVER_STRING, 0xffffff, gameOverH_, false);
	}
	// �Q�[���N���A
	if (isGameClear_)
	{
		int width = GetDrawStringWidthToHandle(GAMECLEAR_STRING, strlen(GAMECLEAR_STRING), gameOverH_);

		// �Q�[���N���A�����̕\��
		DrawStringToHandle((Game::kScreenWidth / 2) - (width / 2), Game::kScreenHeight / 2 + 40,
			GAMECLEAR_STRING, 0xffffff, gameClearH_, false);
	}
	if (preparTimer_ > 0)
	{
		int width = GetDrawStringWidthToHandle(REDY_STRING, strlen(REDY_STRING), readyH_);
		// �����������̕\��
		DrawStringToHandle((Game::kScreenWidth / 2) - (width / 2), Game::kScreenHeight / 2 + 40,
			REDY_STRING, 0xffffff, readyH_, false);
	}

	// �c�@�̕`��
	for (int i = 0; i < life_; i++)
	{
		int x = Game::kScreenWidth / 2 + 100 + (i * 40);

		DrawRectRotaGraph(x, Game::kScreenHeight - 30, 0, 0, 16, 16, 2.0f, 0.0f, lifeH_, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameplayingScene::GameClearUpdate(const InputState& input)
{
	isGameClear_ = true;
	gameClearTimer_++;
	if (gameClearTimer_ % 180 == 0)
	{
		updateFunc_ = &GameplayingScene::FadeOutUpdate;
	}
}

void GameplayingScene::GameOverUpdate(const InputState& input)
{
	if (timer_++ > 60)
	{
		// ���ׂĂ̓G������
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(false);
		}

		if (!pPlayer_->GetAnimeEnd())
		{
			pPlayer_->DeadUpdate();
		}
		else
		{
			isGameOver_ = true;
			gameOverTimer_++;

			if (gameOverTimer_ % 180 == 0)
			{
				updateFunc_ = &GameplayingScene::FadeOutUpdate;
			}
		}
	}
}

void GameplayingScene::PlayerDeadUpdate(const InputState& input)
{
	if (timer_++ > 60)
	{
		// ���ׂĂ̓G������
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(false);
		}

		pPlayer_->DeadUpdate();
	}

	if (pPlayer_->GetAnimeEnd())
	{
		updateFunc_ = &GameplayingScene::FadeOutUpdate;
	}
}

void GameplayingScene::FadeOutUpdate(const InputState& input)
{
	fadeTimer_++;
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));

	ChangeVolumeSoundMem(255 - fadeValue_, bgmSoundH_);

	// �Q�[���I�[�o�[
	if (fadeTimer_ > fade_interval && isGameOver_)
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
	// �Q�[���N���A�[
	else if (fadeTimer_ > fade_interval && isGameClear_)
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
	// �c�@���P������
	else if (fadeTimer_ > fade_interval && life_ > 0)
	{
		for (auto& enemy : pEnemy_)
		{
			enemy->Init();
			enemy->SetInit();
		}

		// ������
		pPlayer_->Init();

		pField_->Init();

		SetInit();

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
	fadeTimer_ = fade_interval;
	fadeValue_ = 255;
	timer_ = 0;

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