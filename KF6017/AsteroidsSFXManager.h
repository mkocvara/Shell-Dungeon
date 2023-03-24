#pragma once
#include "SFXManager.h"

typedef SFXManager Super;

class AsteroidsSFXManager final : public SFXManager
{
public:
	AsteroidsSFXManager(HWND hwnd);
	virtual ~AsteroidsSFXManager();

	void PlayShot();

private:
	SoundIndex mShotSound;

	virtual void LoadAllSounds() override;
};

