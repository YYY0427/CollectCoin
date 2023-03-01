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

/// <summary>
/// �Q�[�����V�[��
/// </summary>
class GameplayingScene : public Scene
{
public:
	GameplayingScene(SceneManager& manager);
	~GameplayingScene(){};

	void Update(const InputState& input);

	void Draw();

	// ���S���̏�����
	void SetInit();

	bool Colision(std::shared_ptr<EnemyBase>enemy);

private:
	//�v���C���[
	std::shared_ptr<Player> pPlayer_;

	// �G
	std::array<std::shared_ptr<EnemyBase>, EnemyBase::enemy_num> pEnemy_;

	//�t�B�[���h
	std::shared_ptr<Field> pField_;

	std::shared_ptr<Map> pMap_;

	//�t�F�[�h�̐F(�f�t�H��)
	unsigned int  fadeColor_ = 0x000000; 

	// �t�F�[�h�ݒ�
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	int clearOrOver_;

	int hartH_;

	bool isAnimeEnd_;

	int timer_;

	// �c�@
	int life_;

	bool faideEnabled_;

	// �t�F�C�h�C��
	void FadeInUpdate(const InputState& input);

	// �A�b�v�f�[�g
	void NormalUpdate(const InputState& input);

	// �t�F�C�h�A�E�g
	void FadeOutUpdate(const InputState& input);

	// �v���C���[���S���̉��o
	void PlayerDeadUpdate(const InputState& input);

	void BoxAnimeUpdate(const InputState& input);

	// �G�����񂾂Ƃ��̉��o
	void EnemyDeadUpdate(const InputState& input);

	// �Q�[���N���A���o
	void GameClearUpdate(const InputState& input);

	// �Q�[���I�[�o�[���o
	void GameOverUpdate(const InputState& input);

	using UpdateFunc_t = void (GameplayingScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};