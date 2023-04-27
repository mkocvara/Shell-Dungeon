#pragma once
#include "SFXManager.h"

class DungeonSFXManager final : public SFXManager
{
	typedef SFXManager Super;

public:
	DungeonSFXManager(HWND hwnd);
	virtual ~DungeonSFXManager();

	void PlayShot();

private:
	SoundIndex mShotSound;

	virtual void LoadAllSounds() override;
};

