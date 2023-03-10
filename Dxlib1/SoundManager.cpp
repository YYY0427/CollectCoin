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
	LoadSoundFile("playerDead", ".wav");
	LoadSoundFile("gameOver", ".wav");
	LoadSoundFile("gameStart", ".ogg");
	LoadSoundFile("gameClear", ".ogg");
}

SoundManager::~SoundManager()
{
}

void SoundManager::Play(const char* name)
{
	PlaySoundMem(nameAndHandleTable_[name], DX_PLAYTYPE_BACK);
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