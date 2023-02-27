#pragma once
#include <memory>
#include "EnemyBase.h"

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
	static constexpr int CHIP_SIZE = 32;

	// �\���ꏊ
	static constexpr int DISPLAY_POS_X = 400;
	static constexpr int DISPLAY_POS_Y = 100;

	// �R���X�g���N�^
	Field();
	 
	// �f�X�g���N�^
	virtual ~Field(){}

	void Init();

	// �X�V����
	void Updata();

	// �`�揈��
	void Draw();
	
	// �|�C���^�̐ݒ�
	void SetEnemy(std::shared_ptr<EnemyBase>enemy, int i) { pEnemy_[i] = enemy; }
	void SetPlayer(std::shared_ptr<Player>player) { pPlayer_ = player; }

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

	// �ړI�n�܂ł̍ŒZ�o�H��T�����߂̏����Ƃ��āA�R�s�[�}�b�v�̏������ƖړI�n�̏ꏊ��0(�ڈ�)��ݒ�
	void MoveDataSet(int playerY, int playerX);

	// �t�B�[���h�Ƀv���C���[����̋��������߂�֐�
	void Search(int y, int x, int goalY, int goalX, int pos);

	// �G������̏ꏊ�ɂ���ꍇ�G�̑���x������t���O�𗧂Ă�֐�
	bool SlowPosition(int y,int x);

	// �G�̃X�|�[���n�_�̊O�ɂ��邩���ɂ��邩
	bool SpornInOrAuto(int y, int x);

	// ����̃u���b�N�ɐN���\��
	bool Intrusion(int y, int x, bool flag);

	// mapData2���Q�Ƃ��ăv���C���[����̐����������������̒l��Ԃ� 
	// �v���C���[���p���[�G�T���擾���Ă�����傫�������̒l��Ԃ�
	int BlinkyMove(int enemyIndexY, int enemyIndexX, bool flag);

	int PinkyMove(int enemyIndexY, int enemyIndexX, bool flag);

	int InkyMove(int enemyIndexY, int enemyIndexX, bool flag);

	int CrydeMove(int enemyIndexY, int enemyIndexX, bool flag);
private:
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<EnemyBase> pEnemy_[EnemyBase::enemy_num];

	int mapData2[Field::MAP_HEIGHT][Field::MAP_WIDTH];

	// �\�����邩�ǂ���
	bool isDraw_;

	// �摜�̃n���h��
	int handle_;
	int coinH_;
	int sordH_;

	int imgIdX_;
	int sordIdx_;

	int pinkyGoalY_;
	int pinkyGoalX_;

	int blinkyGoalY_;
	int blinkyGoalX_;

	int inkyGoalY_;
	int InkyGoalX_;

	int crydeGoalY_;
	int crydeGoalX_;
};