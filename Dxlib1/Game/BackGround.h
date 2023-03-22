#pragma once
#include "../vec2.h"
#include <memory>

class Player;

/// <summary>
/// �w�i
/// </summary>
class BackGround
{
public:
	// �R���X�g���N�^
	BackGround(int handle);

	// �f�X�g���N�^
	virtual~BackGround(){};

	// ����������
	void Init();

	// �X�V����
	void Update(bool getPowerFeed);

	// �`�揈��
	void Draw();

	// �v���C���[�̃|�C���^�̐ݒ�
	void SetPlayer(std::shared_ptr<Player>player) { pPlayer_ = player; }
private:
	// �v���C���[
	std::shared_ptr<Player> pPlayer_;

	// �摜�n���h��
	int handle_;

	// �摜�T�C�Y
	Vec2 size_;

	// ���ꂼ��̕����ɕ\�����閇��
	int width_;
	int height_;

	// �X�N���[��
	int scroll_;
};