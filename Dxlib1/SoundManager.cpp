#include "SoundManager.h"
#include <DxLib.h>
#include <cassert>

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
	LoadSoundFile("coin", ".wav");
	LoadSoundFile("cursor", ".wav");
	LoadSoundFile("decision", ".wav");
	LoadSoundFile("kill", ".wav");
	LoadSoundFile("enemyAttack", ".wav");
	LoadSoundFile("powerUp", ".wav");
	LoadSoundFile("powerDown", ".wav");
	LoadSoundFile("warp", ".wav");
	LoadSoundFile("gameStart", ".ogg");
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