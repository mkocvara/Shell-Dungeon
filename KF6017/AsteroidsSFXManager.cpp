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
	soundEngine->Play(shotSound, true);
}

void AsteroidsSFXManager::LoadAllSounds()
{
	shotSound = soundEngine->LoadWav(L"Assets/shoot.wav");
	allSounds.push_back(shotSound);
}