#pragma once
#include "SFXManager.h"

class DungeonSFXManager final : public SFXManager
{
	typedef SFXManager Super;

public:
	DungeonSFXManager(HWND hwnd);
	virtual ~DungeonSFXManager();

	void PlayKnightHit();
	void PlayOrcHit();
	void PlaySwing();
	void PlayInterface();
	void PlaySwordUnsheath();
	void PlayBoonAcquired();
	void PlayKnightDeath();
	void PlayLevelCleared();

private:
	static const int mKnightHitSoundsNum = 2;
	SoundIndex mKnightHitSounds[mKnightHitSoundsNum];

	static const int mOrcHitSoundsNum = 5;
	SoundIndex mOrcHitSounds[mOrcHitSoundsNum];

	static const int mSwingSoundsNum = 3;
	SoundIndex mSwingSounds[mSwingSoundsNum];
	
	SoundIndex mInterfaceSound;
	SoundIndex mBoonSound;
	SoundIndex mSwordUnsheathSound;
	SoundIndex mKnightDeathSound;
	SoundIndex mLevelClearedSound;

	virtual void LoadAllSounds() override;
};

