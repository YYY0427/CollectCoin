#pragma once
#include <memory>

class Player;
class EnemyBase;

class Field
{
public:
	// �}�b�v�̉���
	static constexpr int MAP_WIDTH = 19;

	// �}�b�v�̏c��
	static constexpr int MAP_HEIGHT = 22;

	// �u���b�N�̑傫��
	static constexpr int BLOCK_SIZE = 32;

	// �\���ꏊ
	static constexpr int DISPLAY_POS_X = 400;
	static constexpr int DISPLAY_POS_Y = 100;

	// �R���X�g���N�^
	Field();
	 
	// �f�X�g���N�^
	~Field(){}

	// �X�V����
	void Updata();

	// �`�揈��
	void Draw();
	void PowerFeedFlash(int y, int x);

	void SetEnemy(std::shared_ptr<EnemyBase>enemy, int i) { pEnemy_[i] = enemy; }
	void SetPlayer(std::shared_ptr<Player>player) { pPlayer_ = player; }

	// �p���[�G�T�̓_��
	void Flash();

	// �Q�[���N���A�`�F�b�N����
	bool IsGameClearCheck();

	// �u���b�N�����邩�ǂ���
	bool IsBlock(int y, int x );

	// �a�����邩�ǂ���
	bool IsFeed(int y, int x);

	// �p���[�G�T�����邩�ǂ���
	bool IsPowerFeed(int y, int x);

	// �w��̈ʒu�Ƀv���C���[������ꍇ
	// �w��̈ʒu�Ƀ��[�v������
	int  Worp(int ky, int kx, int x, int y);

	// player�܂ł̍ŒZ�o�H��T�����߂̏����Ƃ��āA�R�s�[�}�b�v�̏������ƖړI�n(player)�̏ꏊ��10(�ڈ�)��ݒ�
	void MoveDataSet(int playerY, int playerX);

	// �t�B�[���h�Ƀv���C���[����̋��������߂�֐�
	void Search(int y, int x, int pos);

	// mapData2���Q�Ƃ��ăv���C���[����̐����������������̒l��Ԃ� 
	// �v���C���[���p���[�G�T���擾���Ă�����傫�������̒l��Ԃ�
	int BlinkyMove(int enemyIndexY, int enemyIndexX);

	int PinkyMove(int enemyIndexY, int enemyIndexX);

	int InkyMove(int enemyIndexY, int enemyIndexX);

	int CrydeMove(int enemyIndexY, int enemyIndexX);
private:
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<EnemyBase> pEnemy_[4];

	int mapData2[MAP_HEIGHT][MAP_WIDTH];

	// �p���[�G�T�̓_��
	int blendCount_;
	bool blendLimitMax_;

	// �\�����邩�ǂ���
	bool isDraw_;

	// �p���[�G�T�̃n���h��
	int handle_;

	int pinkyGoalY_;
	int pinkyGoalX_;

	int blinkyGoalY_;
	int blinkyGoalX_;

	int InkyGoalY_;
	int InkyGoalX_;

	int crydeGoalY_;
	int crydeGoalX_;
};