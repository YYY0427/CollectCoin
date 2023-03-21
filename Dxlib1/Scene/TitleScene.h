#pragma once
#include "Scene.h"
#include "../vec2.h"
#include <memory>

class InputState;
class BackGround;
class Player;

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public Scene
{
public:
	// �R���X�g���N�^
	TitleScene(SceneManager& manager);

	// �f�X�g���N�^
	~TitleScene();

	// ����������
	void Init();

	// �I������
	void End();

	// �X�V����
	void Update(const InputState& input);

	// �`�揈��
	void Draw();

	// �J�[�\���̈ړ�����
	void CursorMove(int selection);
private:

	// �v���C���[
	std::shared_ptr<Player> pPlayer_;

	// �w�i
	std::shared_ptr<BackGround> pBackGround_;

	// �I���� 
	enum 
	{
		start,
		option,
		credit,
		exit
	};

	// ���W
	Vec2 pos_;
	Vec2 cursor1Pos_;
	Vec2 cursor2Pos_;
	Vec2 cursor3Pos_;
	Vec2 cursor4Pos_;

	// �摜�n���h��
	int playerH_;
	int sordH_;
	int nowaponPlayerH_;
	int waponPlayerH_;
	int skeletonH_;
	int slimeH_;
	int ghostH_;
	int golemH_;
	int controllerH_;
	int cursor1H_;
	int cursor2H_;
	int cursor3H_;
	int cursor4H_;
	int playH_;
	int settingH_;
	int questionH_;
	int doorH_;
	int backGraphH_;

	// �摜�C���f�b�N�X
	int playerImgIdx_;
	int skeletonImgIdx_;
	int slimeImgIdx_;
	int ghostImgIdx_;
	int golemImgIdx_;
	int imgY_;

	// �t�H���g�n���h��
	int titleH_;
	int teachH_;

	// �t�F�[�h�^�C�}�[
	int fadeTimer_;	

	// ����`�Ƃ̃u�����h�
	int fadeValue_;	

	// ���ݑI�𒆂̃C���f�b�N�X
	int currentInputIndex_;

	// �ǂ̃C���f�b�N�X�Ō����������
	int decisionIndex_;

	// �^�[�������邩�ǂ���
	bool isTurnFlag_;

	// ���݂��邩�ǂ���
	bool isEnabled_;

	// �t�F�[�h�C���̎���Update�֐�
	void FadeInUpdate(const InputState& input);

	// �ʏ��Ԃ�Update�֐�
	void NormalUpdate(const InputState& input);

	// �t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdate(const InputState& input);

	// Update�p�����o�֐��|�C���^
	void (TitleScene::* updateFunc_)(const InputState& input);
};