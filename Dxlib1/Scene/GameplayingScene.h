#pragma once
#include "Scene.h"
#include "../Game/EnemyBase.h"
#include "../vec2.h"
#include <memory>	
#include <array>	
#include <vector>

// �v���g�^�C�v�錾
class Player;	
class Field;	
class EnemyBase;
class Map;
class BackGround;
class Coin;
class Particle;

/// <summary>
/// �Q�[�����V�[��
/// </summary>
class GameplayingScene : public Scene
{
public:
	GameplayingScene(SceneManager& manager);
	~GameplayingScene();

	void Update(const InputState& input);

	void Draw();

	// ���S���̏�����
	void SetInit();

	bool Colision(std::shared_ptr<EnemyBase>enemy, int width, int height);

	void StageCheck(int stage);

private:
	enum
	{
		tutorial,	// �`���[�g���A��
		stage1,		// �X�e�[�W�P
	};

	// �X�e�[�W
	int stage_;

	int enemyNum_;

	//�v���C���[
	std::shared_ptr<Player> pPlayer_;

	// �G
	std::vector<std::shared_ptr<EnemyBase>> pEnemy_;

	//�t�B�[���h
	std::shared_ptr<Field> pField_;

	// �}�b�v�`�b�v
	std::shared_ptr<Map> pMap_;

	// �w�i
	std::shared_ptr<BackGround> pBackGround_;

	// �Q�[���N���A���o�R�C��
	std::vector<std::shared_ptr<Coin>> pCoin_;

	// �Q�[���N���A���o�p�[�e�B�N��
	std::array<std::shared_ptr<Particle>, 100> pParticle_;

	//�t�F�[�h�̐F(�f�t�H��)
	unsigned int  fadeColor_ = 0x000000; 

	// �t�F�[�h�ݒ�
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	// BGM�̃t�F�[�h�ݒ�
	static constexpr int bgm_fade_interval = 60;
	int bgmFadeTimer_ = fade_interval;
	int bgmFadeValue_ = 255;

	// �Q�[���I�[�o�[�����̃t�F�[�h�ݒ�
	static constexpr int game_over_fade_interval = 60;
	int gameOverFadeTimer_ = 0;
	int gameOverFadeValue_ = 0;

	// �Q�[���N���A�����̃t�F�[�h�ݒ�
	static constexpr int game_clear_fade_interval = 30;
	int gameClearFadeTimer_ = 0;
	int gameClearFadeValue_ = 0;

	// �����̃n���h��
	int gameOverH_;
	int gameOverShadowH_;
	int gameClearH_;
	int gameClearShadowH_;
	int readyH_;

	// �摜�̃n���h��
	int lifeH_;
	int coinH_;

	int playerStartPosX_;
	int playerStartPosY_;
	int skeletonStartPosX_;
	int skeletonStartPosY_;
	int slimeStartPosX_;
	int slimeStartPosY_;
	int ghostStartPosX_;
	int ghostStartPosY_;
	int golemStartPosX_;
	int golemStartPosY_;

	// �Q�[���I�[�o�[��
	bool isGameOver_;

	// �Q�[���N���A��
	bool isGameClear_;

	// �t�F�C�h�C������
	bool faideEnabled_;

	// �A�j���[�V�������I��������
	bool isAnimeEnd_;

	int timer_;

	int preparTimer_;

	int gameOverTimer_;
	int gameClearTimer_;

	bool enemyKill_;

	bool playerDeadSound_;

	int tempScreenH_;

	int quakeTimer_;
	float quakeX_;
	float quakeY_;

	// �c�@
	int life_;

	// �t�F�C�h�C��
	void FadeInUpdate(const InputState& input);

	// �A�b�v�f�[�g
	void NormalUpdate(const InputState& input);

	// �t�F�C�h�A�E�g
	void FadeOutUpdate(const InputState& input);

	void PrepareUpdate(const InputState& input);

	// �v���C���[���S���̉��o
	void PlayerDeadUpdate(const InputState& input);

	// �G�����񂾂Ƃ��̉��o
	void EnemyDeadUpdate(const InputState& input);

	// �Q�[���N���A���o
	void GameClearUpdate(const InputState& input);

	// �Q�[���I�[�o�[���o
	void GameOverUpdate(const InputState& input);

	using UpdateFunc_t = void (GameplayingScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};