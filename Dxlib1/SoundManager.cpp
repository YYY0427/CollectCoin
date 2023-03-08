#include "SoundManager.h"
#include <DxLib.h>
#include <cassert>

int SoundManager::LoadSoundFile(const char* fileName)
{
	std::string path = "Data/sound/SE/";
	path += fileName;
	path += ".wav";
	int handle = LoadSoundMem(path.c_str());
	assert(handle >= 0);
	nameAndHandleTable_[fileName] = handle;
	return handle;
}

SoundManager::SoundManager()
{
	LoadSoundFile("coin");
	LoadSoundFile("cursor");
	LoadSoundFile("decision");
	LoadSoundFile("kill");
	LoadSoundFile("enemyAttack");
	LoadSoundFile("powerUp");
	LoadSoundFile("powerDown");
	LoadSoundFile("playerDead");
}

SoundManager::~SoundManager()
{
}

void SoundManager::Play(const char* name)
{
	PlaySoundMem(nameAndHandleTable_[name], DX_PLAYTYPE_BACK);
}
