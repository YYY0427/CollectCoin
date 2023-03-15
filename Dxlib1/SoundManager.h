#pragma once
#include <unordered_map>
#include <string>

/// <summary>
/// SE�̊Ǘ����s���V���O���g���N���X
/// </summary>
class SoundManager
{
private:
	int volumeSE_ = 255;
	int volumeBGM_ = 255;

	std::unordered_map<std::string, int> nameAndHandleTable_;
	int LoadSoundFile(const char* fileName, const char* ext);
	SoundManager();
	~SoundManager();

	// �R�s�[��������֎~����
	SoundManager(const SoundManager&) = delete;
	void operator = (const SoundManager&) = delete;
public:
	/// <summary>
	/// SoundManager�g�p�҂�GetInstance()��ʂ����Q�Ƃ��炵�����p�ł��Ȃ�
	/// </summary>
	/// <returns></returns>
	static SoundManager& GetInstance()
	{
		// �B��̎���
		static SoundManager instance;

		// ����̎Q�Ƃ�Ԃ�
		return instance;
	}

	/// <summary>
	/// �w��̃T�E���h��炷
	/// </summary>
	/// <param name="name">�T�E���h��</param>
	void Play(const char* name);		// SE
	void PlayMusic(const char* path);	// BGM
	void PlayJingle(const char* path);	// �W���O��(�v���C���[���S�A�Q�[���N���A�A�Q�[���I�[�o�[�̉�)

	/// <summary>
	/// SE�̃{�����[����ݒ肷��
	/// </summary>
	/// <param name="volume"></param>
	void SetSEVolume(int volume);
	int GetSEVolume() const;

	/// <summary>
	/// BGM�̃{�����[����ݒ肷��
	/// </summary>
	/// <param name="volume"></param>
	void SetBGMVolume(int volume);
	int GetBGMVolume() const;

	/// <summary>
	/// �w��̃T�E���h���Đ������`�F�b�N
	/// </summary>
	/// <param name="name">�T�E���h��</param>
	/// <returns> true �Đ���  false �Đ�����Ă��Ȃ�</returns>
	bool Check(const char* name);

	/// <summary>
	/// �w��̃T�E���h�̉��ʂ̕ύX
	/// </summary>
	/// <param name="name">�T�E���h���A����</param>
	void ChangeVolume(const char* name, int volume);
};