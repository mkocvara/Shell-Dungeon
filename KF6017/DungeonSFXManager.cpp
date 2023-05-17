#include "DungeonSFXManager.h"
#include "mysoundengine.h"
#include "RandUtil.h"


// PUBLIC

DungeonSFXManager::DungeonSFXManager(HWND hwnd) : Super(hwnd)
{
	LoadAllSounds();
}

DungeonSFXManager::~DungeonSFXManager()
{
}

void DungeonSFXManager::PlayKnightHit()
{
	mpSoundEngine->Play(mKnightHitSounds[RandUtil::randRangeInt(0, mKnightHitSoundsNum - 1)]);
}

void DungeonSFXManager::PlayOrcHit()
{
	mpSoundEngine->Play(mOrcHitSounds[RandUtil::randRangeInt(0, mOrcHitSoundsNum - 1)]);
}

void DungeonSFXManager::PlaySwing()
{
	mpSoundEngine->Play(mSwingSounds[RandUtil::randRangeInt(0, mSwingSoundsNum - 1)]);
}

void DungeonSFXManager::PlayInterface()
{
	mpSoundEngine->Play(mInterfaceSound);
}

void DungeonSFXManager::PlaySwordUnsheath()
{
	mpSoundEngine->Play(mSwordUnsheathSound);
}

void DungeonSFXManager::PlayBoonAcquired()
{
	mpSoundEngine->Play(mBoonSound);
}

void DungeonSFXManager::PlayKnightDeath()
{
	mpSoundEngine->Play(mKnightDeathSound);
}

void DungeonSFXManager::PlayLevelCleared()
{
	mpSoundEngine->Play(mLevelClearedSound);
}


// PRIVATE

void DungeonSFXManager::LoadAllSounds()
{
	SoundIndex loadedSound;
	std::wstring path;

	for (int i = 0; i < mKnightHitSoundsNum; i++)
	{
		path = L"Assets/Sound/knight_hit" + std::to_wstring(i + 1) + L".wav";
		loadedSound = mpSoundEngine->LoadWav(path.c_str());
		mKnightHitSounds[i] = loadedSound;
		mAllSounds.push_back(loadedSound);
	}
	for (int i = 0; i < mOrcHitSoundsNum; i++)
	{
		path = L"Assets/Sound/orc_hit" + std::to_wstring(i + 1) + L".wav";
		loadedSound = mpSoundEngine->LoadWav(path.c_str());
		mOrcHitSounds[i] = loadedSound;
		mAllSounds.push_back(loadedSound);
	}
	for (int i = 0; i < mSwingSoundsNum; i++)
	{
		path = L"Assets/Sound/swing" + std::to_wstring(i + 1) + L".wav";
		loadedSound = mpSoundEngine->LoadWav(path.c_str());
		mSwingSounds[i] = loadedSound;
		mAllSounds.push_back(loadedSound);
	}

	path = L"Assets/Sound/interface.wav";
	loadedSound = mpSoundEngine->LoadWav(path.c_str());
	mInterfaceSound = loadedSound;
	mAllSounds.push_back(loadedSound);

	path = L"Assets/Sound/boon.wav";
	loadedSound = mpSoundEngine->LoadWav(path.c_str());
	mBoonSound = loadedSound;
	mAllSounds.push_back(loadedSound);

	path = L"Assets/Sound/sword_unsheath.wav";
	loadedSound = mpSoundEngine->LoadWav(path.c_str());
	mSwordUnsheathSound = loadedSound;
	mAllSounds.push_back(loadedSound);

	path = L"Assets/Sound/knight_death.wav";
	loadedSound = mpSoundEngine->LoadWav(path.c_str());
	mKnightDeathSound = loadedSound;
	mAllSounds.push_back(loadedSound);

	path = L"Assets/Sound/level_clear.wav";
	loadedSound = mpSoundEngine->LoadWav(path.c_str());
	mLevelClearedSound = loadedSound;
	mAllSounds.push_back(loadedSound);
}