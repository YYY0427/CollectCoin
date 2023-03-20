#include "SoundManager.h"
#include <DxLib.h>
#include <cassert>
#include <stdio.h>

namespace
{
//	const char* const SOUND_FILE_SIGNATURE = "SND_";
	constexpr char SOUND_FILE_SIGNATURE[] = "SND_";
//	const char* const SOUND_CONFIG_FILE_PATH = "./sound.conf";
	constexpr char SOUND_CONFIG_FILE_PATH[] = "./sound.conf";
	constexpr float SOUND_CONFIG_VERSION = 1.0f;
}

int SoundManager::LoadSoundFile(const char* fileName, const char* ext)
{
	std::string path = "Data/sound/SE/";
	path += fileName;
	path += ext;
	int handle = LoadSoundMem(path.c_str());
	assert(handle >= 0);
	nameAndHandleTable_[fileName] = handle;
	return handle;
}

SoundManager::SoundManager()
{
	LoadSoundConfig();
	LoadSoundFile("coin", ".wav");
	LoadSoundFile("cursor", ".wav");
	LoadSoundFile("decision", ".wav");
	LoadSoundFile("kill", ".wav");
	LoadSoundFile("enemyAttack", ".wav");
	LoadSoundFile("powerUp", ".wav");
	LoadSoundFile("powerDown", ".wav");
	LoadSoundFile("warp", ".wav");
	LoadSoundFile("gameStart", ".ogg");
	SetSEVolume(volumeSE_);
	SetBGMVolume(volumeBGM_);
}

void SoundManager::LoadSoundConfig()
{
	SoundConfigInfo conf = {};
	FILE* fp = nullptr;
	fopen_s(&fp, SOUND_CONFIG_FILE_PATH, "rb");
	if (fp)
	{
		fread(&conf, sizeof(conf), 1, fp);
		fclose(fp);
		volumeBGM_ = conf.volumeBGM;
		volumeSE_ = conf.volumeSE;
	}
	else
	{
		// メッセージボックスを使って警告表示
		MessageBox(NULL, "ファイルオープン失敗", " ", MB_OK);	

		conf.volumeBGM = 255;
		conf.volumeSE = 255;
		conf.version = 1.0f;

		SaveSoundConfig();
	}
}

SoundManager::~SoundManager()
{
	
}

void SoundManager::Play(const char* name)
{
	PlaySoundMem(nameAndHandleTable_[name], DX_PLAYTYPE_BACK);
}

void SoundManager::PlayMusic(const char* path)
{
	DxLib::PlayMusic(path, DX_PLAYTYPE_LOOP);
	SetVolumeMusic(volumeBGM_);
}

void SoundManager::PlayJingle(const char* path)
{
	DxLib::PlayMusic(path, DX_PLAYTYPE_BACK);
	SetVolumeMusic(volumeBGM_);
}

void SoundManager::SetSEVolume(int volume)
{
	for (auto& record : nameAndHandleTable_)
	{
		ChangeVolumeSoundMem(volume, record.second);
	}
	volumeSE_ = volume;
}

int SoundManager::GetSEVolume() const
{
	return volumeSE_;
}

void SoundManager::SetBGMVolume(int volume)
{
	SetVolumeMusic(volume);
	volumeBGM_ = volume;
}

int SoundManager::GetBGMVolume() const
{
	return volumeBGM_;
}

bool SoundManager::Check(const char* name)
{
	bool sound = CheckSoundMem(nameAndHandleTable_[name]);
	return sound;
}

void SoundManager::ChangeVolume(const char* name, int volume)
{
	ChangeVolumeSoundMem(volume, nameAndHandleTable_[name]);
}

void SoundManager::SaveSoundConfig()
{
	SoundConfigInfo conf = {};
	auto signatureSize = sizeof(conf.signature);
	auto signatureConstSize = sizeof(SOUND_FILE_SIGNATURE);
	memcpy_s(conf.signature, signatureSize, SOUND_FILE_SIGNATURE, signatureConstSize);

	conf.version = SOUND_CONFIG_VERSION;
	conf.volumeBGM = volumeBGM_;
	conf.volumeSE = volumeSE_;

	FILE* fp = nullptr;
	fopen_s(&fp, SOUND_CONFIG_FILE_PATH, "wb");
	if (fp)
	{
		fwrite(&conf, sizeof(conf), 1, fp);
		fclose(fp);
	}
}
