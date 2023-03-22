#pragma once
#include "Scene.h"
#include "../Game/EnemyBase.h"
#include "../vec2.h"
#include <memory>	
#include <array>	
#include <vector>
#include <list>

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
	// �R���X�g���N�^
	GameplayingScene(SceneManager& manager);

	// �f�X�g���N�^
	~GameplayingScene();
	
	// ����������
	void Init();

	// �I������
	void End();

	// �X�V����
	void Update(const InputState& input);

	// �`�揈��
	void Draw();

	// �X�e�[�W�ύX��������
	void SetInit();

	// ���S���̏�����
	void SetDeadInit();

	// �G�ƃv���C���[�̓����蔻��
	bool Colision(std::shared_ptr<EnemyBase>enemy, int width, int height);

	// �X�e�[�W�ɂ���Ēl��ς���
	void StageCheck(int stage);

private:
	// �X�e�[�W
	enum
	{
		tutorial,	// �`���[�g���A��
		stage1,		// �X�e�[�W�P
		stage_num
	};

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
	std::list<std::shared_ptr<Coin>> pCoin_;

	// �Q�[���N���A���o�p�[�e�B�N��
	std::array<std::shared_ptr<Particle>, 100> pParticle_;

	//�t�F�[�h�̐F(�f�t�H��)
	unsigned int  fadeColor_ = 0x000000; 

	// �t�F�[�h�ݒ�
	int fadeTimer_;
	int fadeValue_;

	// BGM�̃t�F�[�h�ݒ�
	int bgmFadeTimer_ ;
	int bgmFadeValue_ ;
					  
	// �Q�[���I�[�o�[�����̃t�F�[�h�ݒ�
	int gameOverFadeTimer_;
	int gameOverFadeValue_;

	// �Q�[���N���A�����̃t�F�[�h�ݒ�
	int gameClearFadeTimer_;
	int gameClearStringFadeValue_;

	// �������W
	Vec2 gameClearPos_;

	// �����̃n���h��
	int gameOverH_;
	int gameOverShadowH_;
	int gameClearH_;
	int gameClearShadowH_;
	int readyH_;

	// �摜�̃n���h��
	int lifeH_;
	int coinH_;
	int tempScreenH_;
	int skeletonH_;
	int slimeH_;
	int ghostH_;
	int golemH_;
	int playH_;
	int retryH_;
	int backH_;
	int cursor1H_;
	int cursor2H_;
	int cursor3H_;
	int cursor4H_;
	int nowaponPlayerH_;
	int waponPlayerH_;
	int deadPlayerH_;
	int attackPlayerH_;
	int mapChipH_;
	int backGraphH_;
	int sordH_;
	int doorH_;

	// �������W
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

	// �J�[�\���̍��W
	Vec2 cursor1Pos_;
	Vec2 cursor2Pos_;
	Vec2 cursor3Pos_;
	Vec2 cursor4Pos_;

	// �Q�[���N���A�����̕��ƍ���
	int stringWidth_;
	int stringHeight_;

	// �Q�[���I�[�o�[��
	bool isGameOver_;

	// �Q�[���N���A��
	bool isGameClear_;

	// �^�C�g����ʂɖ߂�
	bool isTitile_;

	// ���̃X�e�[�W��
	bool isNextStage_;

	// ���g���C���邩
	bool isRetry_;

	// �R�C���̑���
	bool isCoinEnabled_;

	// �t�F�C�h�C������
	bool faideEnabled_;

	// �A�j���[�V�������I��������
	bool isAnimeEnd_;

	// �ǂ̃X�e�[�W��
	int stage_;

	// �G�����̕\�������邩 
	int enemyNum_;

	// �I�����C���f�b�N�X
	int currentInputIndex_;

	// �^�C�}�[
	int waitTimer_;
	int preparTimer_;
	int gameOverTimer_;
	int gameClearTimer_;

	// �v���C���[���S�W���O����炷��
	bool playerDeadSound_;

	// ��ʗh��
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

	// REDY�\���p�A�b�v�f�[�g
	void PrepareUpdate(const InputState& input);

	// �v���C���[���S���̉��o
	void PlayerDeadUpdate(const InputState& input);

	// �G�����񂾂Ƃ��̉��o
	void EnemyDeadUpdate(const InputState& input);

	// �Q�[���N���A���o
	void GameClearUpdate(const InputState& input);

	// �Q�[���I�[�o�[���o
	void GameOverUpdate(const InputState& input);

	// ��ʂ�h�炷
	void Quake();

	// �J�[�\���̈ړ�
	void CursorMove();

	// �����o�֐��|�C���^
	using UpdateFunc_t = void (GameplayingScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};