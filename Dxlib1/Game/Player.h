#pragma once
#include <memory>	
#include "../Geometry.h"
#include "../vec2.h"
#include "EnemyBase.h"

class Field;
class InputState;
class EnemyBase;

class Player
{
public:
	// �摜�̕�
	static constexpr int WIDTH = 32;

	// �摜�̍���
	static constexpr int HEIGHT = 32;

	// �摜�̊g�嗦
	static constexpr float SCALE = 2.2f;

	// 1���ɕK�v�ȃt���[����
	static constexpr int ANIME_FRAME_SPEED = 5;			// �ʏ펞
	static constexpr int DEAD_ANIME_FRAME_SPEED = 10;	// ���S��
	static constexpr int ATTACK_ANIME_FRAME_SPEED = 10;	// �U����

	// �A�j���[�V��������
	static constexpr int ANIME_FRAME_NUM = 8;			// �ʏ펞
	static constexpr int DEAD_ANIME_FRAME_NUM = 8;		// ���S��
	static constexpr int ATTACK_ANIME_FRAME_NUM = 6;	// �U����

	// �R���X�g���N�^
	Player(int normalH, int waponH, int deadH, int attackH, int indexX, int indexY);

	//�f�X�g���N�^
	~Player();

	// �|�C���^�̐ݒ�
	void SetEnemy(std::shared_ptr<EnemyBase>enemy, int i) { pEnemy_[i] = enemy; }
	void SetField(std::shared_ptr<Field>field) { pField_ = field; }

	// ������
	void Init();

	// ����
	void Update(const InputState& input);	// �m�[�}��
	void DeadUpdate();						// �v���C���[�����񂾏ꍇ�̏��� 
	void EnemyKillUpdate();					// �G��|�����Ƃ��̏���
	void ClearUpdate();						// �Q�[���N���A���̏���

	// �`��
	void Draw();

	// �����蔻��
	bool Colision(int direction);

	// �p���[�G�T���擾�����ꍇ�̑��x�v�Z
	void SpeedCalculation();

	// �C���f�b�N�X���W�����W�ɕϊ�
	void PosCalculation();

	// BGM��炷
	void StartMusic();

	// BGM���~�߂�
	void StopMusic();

	// �O����l������
	Vec2 GetSize() const { return deathImgSize_; }
	Vec2 GetPos() const { return pos_; }
	int GetIndexX() const { return indexX_; }
	int GetIndexY() const { return indexY_; }
	bool GetPowerFeed() const { return isPowerFeed_; }
	bool GetDead() const { return isDead_; }
	bool GetAnimeEnd() const { return isAnimeEnd_; }

	// �O����̒l�̐ݒ�
	void SetAnimeEnd(bool isAnimeEnd) { isAnimeEnd_ = isAnimeEnd; }
	void SetKill(bool enemyKill) { enemyKill_ = enemyKill; }
	void SetAttackIdx(int attackImgIdx) { attackImgIdx_ = attackImgIdx; }
	void SetDead(bool isDead) { isDead_ = isDead; }
	void SetEnabled(bool isEnabled) { isEnabled_ = isEnabled; }

	// ����
	enum Direct
	{
		// 1����J�E���g
		up = 1,
		down,
		left,
		right,
		directNum
	};

private:

	// �|�C���^
	std::shared_ptr<Field> pField_;	
	std::shared_ptr<EnemyBase> pEnemy_[EnemyBase::enemy_num];

	Vec2 deathImgSize_;
	Vec2 attackImgSize_;

	// �v���C���[�̍��W
	Vec2 pos_;

	Vec2 vec_;

	int clearTimer_;

	bool enemyKill_;

	//�v���C���[�̃C���f�b�N�X���W
	int indexX_;
	int indexY_;

	// �v���C���[�̉��̍��W
	int kX_;
	int kY_;

	bool isTurnFlag_;
	bool isFlash_;

	int timer_;
	int powerUpBgmH_;

	int soundVolume_;

	int normalBgmVolume_;
	int powerUpBgmVolume_;

	bool soundMin_;

	//�v���C���[�̉摜
	int handle_;
	int normalH_;
	int waponH_;
	int deathH_;
	int attackH_;

	// �ړ��C���^�[�o���p�^�C�}�[
	int moveTimer_;

	// �\������摜�̃C���f�b�N�X
	int imgIdX_;
	int deadImgIdx_;
	int attackImgIdx_;

	// �摜�̕����̐؂�ւ�
	float angle_;

	// �v���C���[���ǂ̕����ɐi��ł��邩
	int moveDirection_;

	// �v���C���[���ǂ̕����ɐi�݂�����
	int wantMoveDirection_;

	// �擾�����G�T�̐�
	int feedGetNum_;

	// �v���C���[�̈ړ����x
	int moveInterval_;

	// �p���[�G�T���擾�������ǂ���
	bool isPowerFeed_;

	// �p���[�G�T���擾������ԂŃp���[�G�T���擾������
	bool isPowerFeed2_;
	
	// �p���[�G�T���擾������Ԃłǂ̂��炢�o������
	int powerFeedTimer_;

	// �p���[�G�T���擾�����ꍇ�̃X�s�[�h
	float speed2_;

	// �v���C���[���G�Ɠ���������
	bool isDead_;

	// ���S���A�j���[�V�������I�����邩
	bool isAnimeEnd_;

	float speed_;

	bool isEnabled_;

	// ����̃u���b�N�ɐN���\��
	bool isIntrusion_;

	int imgY_;
};