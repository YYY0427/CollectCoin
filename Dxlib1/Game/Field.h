#pragma once
#include <memory>
#include <vector>
#include "EnemyBase.h"

class Player;
class EnemyBase;

class Field
{
public:
	// �u���b�N�̑傫��
	static constexpr int CHIP_SIZE = 32;

	// �\���ꏊ
	static constexpr int DISPLAY_POS_X = 495;
	static constexpr int DISPLAY_POS_Y = 100;

	// �X�e�[�W�̃T�C�Y�̎擾
	int GetMapDataY() const { return mapHeight_; }
	int GetMapDataX() const { return mapWidth_; }

	// �R���X�g���N�^
	Field(int sordH, int doorH, int coinH, int stage);
	 
	// �f�X�g���N�^
	virtual ~Field();

	// �v���C���[���S���̏�����
	void Init();

	// �X�e�[�W�ɂ���Ĕz��̃T�C�Y�̕ύX�A�G�̖ڕW��ύX
	void StageCheck(int stage);
	void StageCheck2(int stage);

	// �X�V����
	void Updata();

	// �`�揈��
	void Draw();
	
	// �|�C���^�̐ݒ�
	void SetEnemy(std::shared_ptr<EnemyBase>enemy, int i) { pEnemy_[i] = enemy; }
	void SetPlayer(std::shared_ptr<Player>player) { pPlayer_ = player; }

	// �}�b�v�f�[�^�̃R�C���̍��v�����̏W�v
	int LestCoin();

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
	int  Warp(int ky, int kx, int x, int y);

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

	// �G�̓���
	int SkeletonMove(int enemyIndexY, int enemyIndexX, bool flag);
	int GolemMove(int enemyIndexY, int enemyIndexX, bool flag);
	int SlimeMove(int enemyIndexY, int enemyIndexX, bool flag);
	int GhostMove(int enemyIndexY, int enemyIndexX, bool flag);
private:
	std::shared_ptr<Player> pPlayer_;
	std::vector<std::shared_ptr<EnemyBase>> pEnemy_;
	std::vector<std::vector<int>> mapData_;
	std::vector<std::vector<int>> enemyTargetMapData_;

	// �X�e�[�W�̃T�C�Y
	int mapHeight_;
	int mapWidth_;
	
	// �X�e�[�W
	int stage_;

	// �\�����邩�ǂ���
	bool isDraw_;

	// �v���C���[���擾�����R�C���̖���
	int getCoinNum_;

	// �v���C���[���W�߂Ȃ���΂����Ȃ��R�C���̖���
	int coinTotalNum_;

	// �h�A���J�����߂邩
	bool openDoor_;
	bool closeDoor_;

	// �摜�̃n���h��
	int coinH_;
	int sordH_;
	int doorH_;

	// �摜�C���f�b�N�X
	int coinImgIdx_;
	int doorImgIdx_;

	// �t�H���g�n���h��
	int stringH_;

	// �G�̖ڕW
	int pinkyGoalY_;
	int pinkyGoalX_;

	int blinkyGoalY_;
	int blinkyGoalX_;

	int inkyGoalY_;
	int inkyGoalX_;

	int crydeGoalY_;
	int crydeGoalX_;
};