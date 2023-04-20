#pragma once
#include "Service.h"
#include "windows.h"
#include <memory>
#include <vector>

class MySoundEngine;

class SFXManager : public Service
{
	typedef Service Super;

public:
	typedef unsigned int SoundIndex;

	SFXManager(HWND hwnd);
	virtual ~SFXManager();

	// Limited to 0 - 100. Represents a percentage of volume applied to the sound engine.
	void SetMasterVolume(int volume);
	void ChangeMasterVolume(int change);
	void IncreaseMasterVolume(int increment);
	void DecreaseMasterVolume(int decrement);

	// Subclass should have functions to play game-specific sounds.

protected:
	std::unique_ptr<MySoundEngine> mpSoundEngine;
	std::vector<SoundIndex> mAllSounds;
	virtual void LoadAllSounds() = 0;

private:
	int mMasterVolume = 100;

	// Subclass should have references to game-specific sounds.
};

