#pragma once
#include "Scene.h"
#include "../Game/EnemyBase.h"
#include <memory>	//�u�X�}�[�g�|�C���^�v���g�����߂�include
#include <array>	//�z��p

//�v���g�^�C�v�錾
class Player;	//�v���C���[�N���X
class Field;	
class EnemyBase;
class Map;
class BackGround;

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

private:
	//�v���C���[
	std::shared_ptr<Player> pPlayer_;

	// �G
	std::array<std::shared_ptr<EnemyBase>, EnemyBase::enemy_num> pEnemy_;

	//�t�B�[���h
	std::shared_ptr<Field> pField_;

	std::shared_ptr<Map> pMap_;

	std::shared_ptr<BackGround> pBackGround_;

	//�t�F�[�h�̐F(�f�t�H��)
	unsigned int  fadeColor_ = 0x000000; 

	// �t�F�[�h�ݒ�
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	// �Q�[���I�[�o�[�����̃t�F�[�h�ݒ�
	static constexpr int game_over_fade_interval = 60;
	int gameOverFadeTimer_ = 0;
	int gameOverFadeValue_ = 0;

	// �����̃n���h��
	int gameOverH_;
	int gameClearH_;
	int readyH_;

	// �摜�̃n���h��
	int lifeH_;

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