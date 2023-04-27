#include "DungeonSFXManager.h"
#include "mysoundengine.h"

DungeonSFXManager::DungeonSFXManager(HWND hwnd) : Super(hwnd)
{
	LoadAllSounds();
}

DungeonSFXManager::~DungeonSFXManager()
{
}

void DungeonSFXManager::PlayShot()
{
	mpSoundEngine->Play(mShotSound);
}

void DungeonSFXManager::LoadAllSounds()
{
	mShotSound = mpSoundEngine->LoadWav(L"Assets/shoot.wav");
	mAllSounds.push_back(mShotSound);
}