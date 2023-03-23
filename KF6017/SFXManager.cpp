#include "SFXManager.h"
#include "mysoundengine.h"
#include <algorithm>
#include "ErrorLogger.h"

SFXManager::SFXManager(HWND hwnd)
{
	soundEngine = std::make_unique<MySoundEngine>(hwnd);
	//LoadAllSounds();
}

SFXManager::~SFXManager()
{
}


// PUBLIC

void SFXManager::SetMasterVolume(int volume)
{
	masterVolume = std::min<int>(100, std::max<int>(0, volume));

	for (SoundIndex soundIndex : allSounds)
	{
		// if 0% set volume to lowest -1000, otherwise set to value
		// between -3465 (almost silent) and 0 (full volume) based
		// on well working volume levels in DX (see SetVolume comment 
		// in mysoundengine.h)
		volume = (volume == 0) ? -10000 : (volume - 100) * 35; 
		if (FAILED(soundEngine->SetVolume(soundIndex, volume)))
			ErrorLogger::Writeln(L"SFXManager::SetMasterVolume() - Failed to set volume of sound with index " + soundIndex);
	}
}

void SFXManager::ChangeMasterVolume(int change)
{
	SetMasterVolume(masterVolume + change);
}

void SFXManager::IncreaseMasterVolume(int increment)
{
	ChangeMasterVolume(increment);
}

void SFXManager::DecreaseMasterVolume(int decrement)
{
	ChangeMasterVolume(decrement);
}
