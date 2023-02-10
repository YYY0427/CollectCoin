#pragma once
#include "Scene.h"
#include <memory>	//�u�X�}�[�g�|�C���^�v���g�����߂�include
#include <array>	//�z��p

//�v���g�^�C�v�錾
class Player;	//�v���C���[�N���X
class Field;	
class EnemyBase;

/// <summary>
/// �Q�[�����V�[��
/// </summary>
class GameplayingScene : public Scene
{
public:
	GameplayingScene(SceneManager& manager);
	~GameplayingScene() {}

	void Update(const InputState& input);

	void Draw();

	bool Colision(std::shared_ptr<EnemyBase>enemy);

private:
	//�v���C���[
	std::shared_ptr<Player> pPlayer_;

	// �G
	std::array<std::shared_ptr<EnemyBase>, 4> pEnemy_;

	//�t�B�[���h
	std::shared_ptr<Field> pField_;

	//�t�F�[�h�̐F(�f�t�H��)
	unsigned int  fadeColor_ = 0x000000; 

	// �t�F�[�h�ݒ�
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	int timer_;

	// �t�F�C�h�C��
	void FadeInUpdate(const InputState& input);

	// �A�b�v�f�[�g
	void NormalUpdate(const InputState& input);

	// �G�����񂾂Ƃ��̉��o
	void EnemyDeadUpdate(const InputState& input);

	// �Q�[���N���A���o
	void GameClearUpdate(const InputState& input);

	// �Q�[���I�[�o�[���o
	void GameOverUpdate(const InputState& input);

	// �Q�[���N���A���̃t�F�C�h�A�E�g
	void GameClearFadeOutUpdate(const InputState& input);

	// �Q�[���I�[�o�[���̃t�F�C�h�A�E�g
	void GameOverFadeOutUpdate(const InputState& input);

	using UpdateFunc_t = void (GameplayingScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};