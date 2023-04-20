#pragma once
#include "SFXManager.h"

class AsteroidsSFXManager final : public SFXManager
{
	typedef SFXManager Super;

public:
	AsteroidsSFXManager(HWND hwnd);
	virtual ~AsteroidsSFXManager();

	void PlayShot();

private:
	SoundIndex mShotSound;

	virtual void LoadAllSounds() override;
};

