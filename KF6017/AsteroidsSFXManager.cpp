#include "AsteroidsSFXManager.h"
#include "mysoundengine.h"

AsteroidsSFXManager::AsteroidsSFXManager(HWND hwnd) : Super(hwnd)
{
	LoadAllSounds();
}

AsteroidsSFXManager::~AsteroidsSFXManager()
{
}

void AsteroidsSFXManager::PlayShot()
{
	mpSoundEngine->Play(mShotSound);
}

void AsteroidsSFXManager::LoadAllSounds()
{
	mShotSound = mpSoundEngine->LoadWav(L"Assets/shoot.wav");
	mAllSounds.push_back(mShotSound);
}